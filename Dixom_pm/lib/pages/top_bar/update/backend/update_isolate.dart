import 'dart:async';
import 'dart:convert';
import 'dart:isolate';
import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/main.dart';
import 'package:flutter/foundation.dart';
import '../../../../driver/device_rx_tx/exchange_param.dart';
import 'update_const.dart';
import 'update_function.dart';
import 'update_model.dart';

SendPort? sendPort;

void updateNotification(CommandMsg msg) {
  if (msg.podParmetr == UpdateStatus.bootloaderUpdatedSucces.index) {
    appRef.read(frimwareUpdateViewPrdr.notifier).update(status: UpdateStatus.updateSystemBlocked);
    //Успешное обноеление загрузчика
    appRef.read(frimwareUpdateViewPrdr.notifier).update(status: UpdateStatus.bootloaderUpdatedSucces);
  } else if (msg.podParmetr == UpdateStatus.bootloaderUpdateFail.index) {
    //Ошибка обновленя загрузчика
    appRef.read(frimwareUpdateViewPrdr.notifier).update(status: UpdateStatus.bootloaderUpdateFail);
  } else if (msg.podParmetr == UpdateStatus.updateSystemBlocked.index) {
    //Заблокирована система обновления
  } else if (msg.podParmetr == UpdateStatus.noBootloaderUpdateRequired.index) {
    //Обновление загрузчика не требуется
    appRef.read(frimwareUpdateViewPrdr.notifier).update(status: UpdateStatus.noBootloaderUpdateRequired);
  }
  sendPort!.send(msg.podParmetr);
}

Future<void> downloadFirmwareIslyate(FirmwareInfoModel frimwareInfo) async {
  appRef.read(frimwareUpdateViewPrdr.notifier).update(status: UpdateStatus.appDownload, progress: 0);

  final firmwareFile = await downloadFirmware(
    url: frimwareInfo.url,
    fileSize: int.parse(frimwareInfo.size),
  );

  if (firmwareFile is UpdateStatus) {
    appRef.read(frimwareUpdateViewPrdr.notifier).update(status: firmwareFile);
    return;
  }

  if (appRef.read(frimwareUpdateViewPrdr.select((value) => value.btnAction == UpdateBtnAction.download))) {
    appRef.read(frimwareUpdateViewPrdr.notifier).update(status: UpdateStatus.cancel, progress: 0);
    return;
  }

  ReceivePort receivePort = ReceivePort();

  Isolate thread = await Isolate.spawn(uploadFirmwareToDevice, [receivePort.sendPort, firmwareFile]);

  receivePort.listen((dynamic message) {
    try {
      if (message is SendPort) {
        //получаем senPort
        sendPort = message;
      } else if (message is UpdateStatus) {
        switch (message) {
          case UpdateStatus.uploadError:
            sendPort!.send(UpdateStatus.cancel.index);
            thread.kill(priority: Isolate.immediate);
            break;
          case UpdateStatus.uploadSucces:
            appRef.read(frimwareUpdateViewPrdr.notifier).update(progress: 0);
            thread.kill(priority: Isolate.immediate);
            break;
          case UpdateStatus.cancel:
            thread.kill(priority: Isolate.immediate);
            break;
          default:
            break;
        }
        appRef.read(frimwareUpdateViewPrdr.notifier).update(status: message);
      } else if (message is double) {
        appRef.read(frimwareUpdateViewPrdr.notifier).update(progress: message);
      } else if (message is CommandMsg) {
        transmitByte(
          data: message.buffer,
          lenght: message.buffer.length,
          timeout: 200,
        );
      }
    } catch (e) {
      // Handle the message appropriately if it is not a CommandMsg.
    }
  });
}

//DescriptionSystem desk = DescriptionSystem();

void uploadFirmwareToDevice(List args) async {
  /*Настройка обмена данными между основным изолятом */
  SendPort portFromIsolateToMain = args[0] as SendPort;
  ReceivePort portFromMainToIsolate = ReceivePort();
  portFromIsolateToMain.send(portFromMainToIsolate.sendPort);
  Uint8List firmwareFile = args[1] as Uint8List;

  /*Выдергиваем информацию о прошивке из файла*/
  String data = utf8.decode(firmwareFile.sublist(1, 250));
  List<String> frimwareInfod = data.split("*");

  bool closeIsolateImmediately = false;

  if (frimwareInfod.length > 10) {
    for (int i = 0; i < frimwareInfod.length; i++) {
      frimwareInfod[i] = frimwareInfod[i].replaceAll(' ', '');
    }
    int firmwareCrc = int.parse(frimwareInfod[LocatInfoInFile.crc]);
    int firmwareAppAddr = int.parse(frimwareInfod[LocatInfoInFile.appAddr]);
    int eraseSector = int.parse(frimwareInfod[LocatInfoInFile.eraseSector]);
    int eraseSectorCount = int.parse(frimwareInfod[LocatInfoInFile.eraseCount]);
    int updateFileSize = firmwareFile.length;
    int firmwareSize = int.parse(frimwareInfod[LocatInfoInFile.size]);

    /*Настрока CRC32 для подсчёта контрольной суммы*/
    List<int> crcTable = List<int>.filled(256, 0);
    int crc;
    for (int i = 0; i < 256; i++) {
      crc = i;
      for (int j = 0; j < 8; j++) {
        crc = (crc & 1) != 0 ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
      }
      crcTable[i] = crc;
    }
    crc = 0xFFFFFFFF;
    for (int i = 256; i < updateFileSize; i++) {
      crc = crcTable[(crc ^ firmwareFile[i]) & 0xFF] ^ (crc >> 8);
    }
    crc ^= 0xFFFFFFFF;

    Completer unlockUpdate = Completer<void>();
    Completer prepairingForUpdate = Completer<void>();
    Completer delivery = Completer<void>();
    Completer frimwareUploadState = Completer<void>();

    /*сокращаем строку */
    //final desc = DescriptionSystem();

    bool succesDelivery = false;

    //Слушать изолят из main
    portFromMainToIsolate.listen((dynamic message) {
      if (message is int) {
        //отмена обновления

        if (message == UpdateStatus.updateSystemUnlock.index) {
          unlockUpdate.complete();
        } else if (message == UpdateStatus.updateSystemBlocked.index) {
          unlockUpdate.completeError("error");
        } else if (message == UpdateStatus.updatePreparationDone.index) {
          prepairingForUpdate.complete();
        } else if (message == UpdateStatus.errorWhilePreparingUpdate.index) {
          prepairingForUpdate.completeError("error");
        } else if (message == UpdateStatus.systemCommandsAreBlocked.index) {
          prepairingForUpdate.completeError("error");
        } else if (message == UpdateStatus.succesVerified.index) {
          frimwareUploadState.complete();
        } else if (message == UpdateStatus.isCorrupted.index) {
          frimwareUploadState.completeError("error");
        } else if (message == UpdateStatus.uploadError.index) {
          frimwareUploadState.completeError("error");
        } else if (message == UpdateStatus.succesDelivery.index) {
          succesDelivery = true;
        } else if (message == UpdateStatus.errorDelivery.index) {
          delivery.completeError("error");
          closeIsolateImmediately = true;
        } else if (message == UpdateStatus.cancel.index) {
          portFromIsolateToMain.send(
            stringToCommandMsg(("${RootPacked.system.index} ${SystemPacked.accesControl.index}")),
          );

          portFromIsolateToMain.send(UpdateStatus.cancel);
        }
      }
    });

    /*Разблокировка системы обновления*/
    int password = 510286;
    portFromIsolateToMain.send(UpdateStatus.appUnlock);
    portFromIsolateToMain.send(
      stringToCommandMsg(("${RootPacked.system.index} ${SystemPacked.accesControl.index} $password")),
    );

    /*Ожидание разблокировки системы обновления*/
    if (await unlockUpdate.future != "error") {
      /*Пдготовка к обновлению*/
      Uint8List sendBuffer = Uint8List(64);
      sendBuffer[0] = 48 + RootPacked.system.index;
      sendBuffer[1] = 32;
      sendBuffer[2] = 48 + SystemPacked.fuPrepairingUpdate.index;
      sendBuffer[3] = 32;
      sendBuffer[4] = firmwareSize;
      sendBuffer[5] = firmwareSize >> 8;
      sendBuffer[6] = firmwareSize >> 16;
      sendBuffer[7] = firmwareSize >> 24;
      sendBuffer[8] = updateFileSize - 256;
      sendBuffer[9] = (updateFileSize - 256) >> 8;
      sendBuffer[10] = (updateFileSize - 256) >> 16;
      sendBuffer[11] = (updateFileSize - 256) >> 24;
      sendBuffer[12] = crc;
      sendBuffer[13] = crc >> 8;
      sendBuffer[14] = crc >> 16;
      sendBuffer[15] = crc >> 24;
      sendBuffer[16] = firmwareCrc;
      sendBuffer[17] = firmwareCrc >> 8;
      sendBuffer[18] = firmwareCrc >> 16;
      sendBuffer[19] = firmwareCrc >> 24;
      sendBuffer[20] = firmwareAppAddr;
      sendBuffer[21] = firmwareAppAddr >> 8;
      sendBuffer[22] = firmwareAppAddr >> 16;
      sendBuffer[23] = firmwareAppAddr >> 24;
      sendBuffer[24] = eraseSector;
      sendBuffer[25] = eraseSectorCount;

      /*Отправка команды, на подготовку платформы к обновлению*/
      portFromIsolateToMain.send(UpdateStatus.appPreparation);
      portFromIsolateToMain.send(byteToCommandMsg(64, sendBuffer));

      /*Ожидание готовности платформы к обновлению*/
      if (await prepairingForUpdate.future != "error") {
        try {
          sendBuffer[2] = 48 + SystemPacked.fuDownload.index;
          int currntBlock = 0;
          /*Загрузка прошивки в платформу, по 58 байт на один пакет*/
          int currentByte = 256;
          succesDelivery = true;
          bool uploadComlit = false;
          bool updateState = false;
          int timeOutDelivery = 0;
          int mode = 0;
          portFromIsolateToMain.send(UpdateStatus.appUpload);

          Timer.periodic(const Duration(microseconds: 1), (timer) async {
            if (mode == 1) {
              ///Если сообщение не доставилось то ошибка обновления
              if (timeOutDelivery > 1000000) {
                portFromIsolateToMain.send(UpdateStatus.uploadError);
                timer.cancel();
              } else {
                timeOutDelivery++;
              }
            }

            if (uploadComlit == false) {
              if (succesDelivery == false) {
                return;
              }
              timeOutDelivery = 0;
              if (currentByte < updateFileSize) {
                sendBuffer[4 + currntBlock] = firmwareFile[currentByte];
                currntBlock++;

                if (currntBlock == 58) {
                  succesDelivery = false;
                  mode = 1;

                  portFromIsolateToMain.send((currentByte / updateFileSize * 100) / 100);
                  currntBlock = 0;

                  portFromIsolateToMain.send(byteToCommandMsg(64, sendBuffer));
                }

                currentByte++;
              } else {
                uploadComlit = true;

                ///Если остались не отправленные данные, отправляем остатки
                if (currntBlock != 0) {
                  portFromIsolateToMain.send(byteToCommandMsg(64, sendBuffer));
                }
                mode = 0;

                portFromIsolateToMain.send(UpdateStatus.appCheck);

                ///Отправляем команду на проверку правильности загрузки прошивки
                portFromIsolateToMain.send(
                  stringToCommandMsg(("${RootPacked.system.index} ${SystemPacked.fuCheckFirmware.index}")),
                );

                ///Ждём пока проверится прошивка на правильность загрузки*/
                if (await frimwareUploadState.future == "error") {
                  portFromIsolateToMain.send(UpdateStatus.uploadError);

                  ///Если чтото не так то ошибка обновления
                } else {
                  updateState = true;

                  int restartTimeOut = 0;

                  ///Если всё хорошо завершаем обновление
                  portFromIsolateToMain.send(UpdateStatus.appDone);

                  Timer.periodic(const Duration(seconds: 1), (timer) async {
                    if (restartTimeOut == updateRestar) {
                      ///Перезагружаем аудиоплатформу
                      portFromIsolateToMain.send(
                        stringToCommandMsg(("${RootPacked.system.index} ${SystemPacked.fuCompleteUpdate.index}")),
                      );
                      portFromIsolateToMain.send(UpdateStatus.uploadSucces);
                      timer.cancel();
                    }
                    portFromIsolateToMain.send((restartTimeOut * 10) / 100);
                    restartTimeOut++;
                  });
                }
                timer.cancel();
              }
            }
          });

          if (updateState) {}
        } catch (e) {
          portFromIsolateToMain.send(UpdateStatus.uploadSucces);
        }
      }
    }
  } else {
    closeIsolateImmediately = true;
  }

  if (closeIsolateImmediately) {
    portFromIsolateToMain.send(UpdateStatus.cancel);
  }
}
