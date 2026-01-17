import 'dart:math';
import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/main.dart';
import 'package:dixom/components/widgets/decorated_container.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../device_info/backend/version.dart';
import '../backend/update_const.dart';
import '../backend/update_isolate.dart';
import '../backend/update_model.dart';
import 'package:flutter_svg/flutter_svg.dart';

class FirmwareUpdateView extends ConsumerWidget {
  const FirmwareUpdateView({super.key});
  static int index = 0;
  static bool delayedCheck = false;

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final devInfo = ref.watch(deviceVersionPrdr);
    final updateView = ref.watch(frimwareUpdateViewPrdr);
    final WorkState connection = ref.watch(connectionState.select((value) => value.status));

    if (connection != WorkState.connected) {
      delayedCheck = false;
    }

    return DecoratedContainer(
      top: 0,
      child: Column(
        children: [
          SvgPicture.asset(
            color: Colors.cyan,
            'assets/images/svg/inspirationLogo.svg',
            width: 300,
            height: 100,
          ),
          const SizedBox(width: 16, height: 16),
          if (connection == WorkState.connected)
            newBoot(devInfo.newBootlodaeFrimware, devInfo.verBootloader)
                ? Text("${devInfo.firmwareName} v${devInfo.verFrimware}", style: const TextStyle(fontSize: 20))
                : Text("Установлен загрузчик v${devInfo.verBootloader}", style: const TextStyle(fontSize: 20)),
          if (connection != WorkState.connected) const Text("Нет подключения", style: TextStyle(fontSize: 20)),
          if (updateView.status == UpdateStatus.appUpdateBootloader) const SizedBox(height: 16),
          if (updateView.status == UpdateStatus.appUpdateBootloader)
            const Text("Обновление загрузчика...", textAlign: TextAlign.center),
          if (updateView.status == UpdateStatus.appUpdateBootloader) const SizedBox(height: 16),
          if (updateView.status == UpdateStatus.appUpdateBootloader) const CircularProgressIndicator(),
          if (delayedCheck == true)
            Consumer(builder: (context, ref, _) {
              final frimware = ref.watch(frimwareInfoPrdr);

              return Expanded(
                child: Column(
                  children: [
                    Expanded(
                      child: newBoot(devInfo.newBootlodaeFrimware, devInfo.verBootloader)
                          ? SingleChildScrollView(
                              child: Center(
                                child: frimware.when(
                                  data: (firmwareInfo) {
                                    if (frimware.isLoading) {
                                      return const Column(
                                        children: [
                                          SizedBox(height: 16),
                                          CircularProgressIndicator(),
                                        ],
                                      );
                                    }

                                    return AnimatedSize(
                                      duration: const Duration(milliseconds: 250),
                                      child: Column(
                                        children: [
                                          const SizedBox(height: 16),
                                          Text(updateView.status.decoding),
                                          if (updateView.btnAction == UpdateBtnAction.cancellation)
                                            const SizedBox(height: 16),
                                          if (updateView.btnAction == UpdateBtnAction.cancellation)
                                            progressBar(updateView),
                                          if (updateView.btnAction != UpdateBtnAction.cancellation)
                                            frimwareList(firmwareInfo, ref),
                                          const SizedBox(height: 16),
                                          frimwareDescription(firmwareInfo),
                                        ],
                                      ),
                                    );
                                  },
                                  loading: () {
                                    return const Column(
                                      children: [
                                        SizedBox(height: 16),
                                        CircularProgressIndicator(),
                                      ],
                                    );
                                  },
                                  error: (error, stackTrace) {
                                    delayedCheck = false;
                                    return Column(
                                      mainAxisAlignment: MainAxisAlignment.end,
                                      children: [
                                        const SizedBox(height: 16),
                                        Text(
                                          UpdateStatus.appDownloadError.decoding,
                                          textAlign: TextAlign.center,
                                        ),
                                        const SizedBox(height: 16),
                                        Text(
                                          error.toString(),
                                          textAlign: TextAlign.center,
                                        ),
                                        const SizedBox(height: 16),
                                        startButton(connection, ref),
                                      ],
                                    );
                                  },
                                ),
                              ),
                            )
                          : const Text(""),
                    ),
                    newBoot(devInfo.newBootlodaeFrimware, devInfo.verBootloader)
                        ? universalButton(frimware, updateView, ref, context, devInfo)
                        : updateBootloaderBtn(ref, devInfo.newBootlodaeFrimware),
                  ],
                ),
              );
            }),
          if (delayedCheck == false) const Expanded(child: Text("")),
          if (delayedCheck == false) startButton(connection, ref),
        ],
      ),
    );
  }

  bool newBoot(String newVersion, String oldVersion) {
    return newVersion == oldVersion;
  }

  ElevatedButton startButton(WorkState connection, WidgetRef ref) {
    return ElevatedButton(
      onPressed: connection == WorkState.connected
          ? () {
              index = 0;
              delayedCheck = true;
              try {
                ref.refresh(frimwareInfoPrdr);
              } catch (e) {}

              ref.read(frimwareUpdateViewPrdr.notifier).update(progress: Random().nextInt(100).toDouble());
            }
          : null,
      child: Text(UpdateBtnAction.getJson.decoding),
    );
  }

  ElevatedButton updateBootloaderBtn(WidgetRef ref, String versionBoot) {
    return ElevatedButton(
      onPressed: ref.read(frimwareUpdateViewPrdr).status == UpdateStatus.appUpdateBootloader
          ? null
          : () {
              ref.read(frimwareUpdateViewPrdr.notifier).update(status: UpdateStatus.appUpdateBootloader);
              ref.read(frimwareUpdateViewPrdr.notifier).updateBootloader();
            },
      child: Text("Обновить загрузчик до $versionBoot"),
    );
  }

  Row universalButton(AsyncValue<FirmwareModel> frimware, UpdateViewModel updateView, WidgetRef ref,
      BuildContext context, DeviceVersionModel devInfo) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        if (updateView.btnAction == UpdateBtnAction.download || updateView.btnAction == UpdateBtnAction.cancellation)
          ElevatedButton(
            onPressed: frimware.isLoading
                ? null
                : () {
                    if (updateView.btnAction == UpdateBtnAction.getJson) {
                      ref.refresh(frimwareInfoPrdr);
                    } else if (updateView.btnAction == UpdateBtnAction.download) {
                      final frimware = ref.read(frimwareInfoPrdr);

                      FirmwareInfoModel? frimwareInfo;

                      frimware.whenData((value) => frimwareInfo = value.firmwareList[index]);

                      showUpdateConfirmation(
                        ref,
                        frimwareInfo!,
                        context,
                        devInfo.verFrimware,
                      );
                    } else if (updateView.btnAction == UpdateBtnAction.cancellation) {
                      appRef
                          .read(frimwareUpdateViewPrdr.notifier)
                          .update(progress: index.toDouble(), btnAction: UpdateBtnAction.download);
                      sendPort!.send(UpdateStatus.cancel.index);
                    }
                  },
            child: Text(updateView.btnAction.decoding),
          ),
        if (updateView.btnAction == UpdateBtnAction.download) const SizedBox(width: 16),
        if (updateView.btnAction == UpdateBtnAction.download)
          IconButton(
            icon: const Icon(Icons.refresh),
            onPressed: frimware.isLoading
                ? null
                : () {
                    ref.refresh(frimwareInfoPrdr);
                  },
          ),
      ],
    );
  }

  DecoratedContainer frimwareDescription(FirmwareModel firmwareInfo) {
    return DecoratedContainer(
      color: Colors.black12,
      child: Text(
          "• Устройство: ${firmwareInfo.device}\n• Размер: ${firmwareInfo.firmwareList[index].size}байт\n• Версия: ${firmwareInfo.firmwareList[index].version}\n• Выуск: ${firmwareInfo.firmwareList[index].date}\n• Описание: ${firmwareInfo.firmwareList[index].description}\n"),
    );
  }

  DropdownButton<FirmwareInfoModel> frimwareList(FirmwareModel firmwareInfo, WidgetRef ref) {
    return DropdownButton<FirmwareInfoModel>(
      value: firmwareInfo.firmwareList[index],
      icon: const Icon(Icons.arrow_downward),
      elevation: 16,
      onChanged: (FirmwareInfoModel? description) {
        index = firmwareInfo.firmwareList.indexOf(description!);

        ref
            .read(frimwareUpdateViewPrdr.notifier)
            .update(progress: index.toDouble(), btnAction: UpdateBtnAction.download);
      },
      items: firmwareInfo.firmwareList.map((FirmwareInfoModel value) {
        return DropdownMenuItem<FirmwareInfoModel>(
          value: value,
          child: Text("${value.name} v${value.version} от ${value.date}"),
        );
      }).toList(),
    );
  }

  Row progressBar(UpdateViewModel updateView) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        if (updateView.status == UpdateStatus.appPreparation || updateView.status == UpdateStatus.appCheck)
          const CircularProgressIndicator(),
        if (updateView.status == UpdateStatus.appUpload ||
            updateView.status == UpdateStatus.appDone ||
            updateView.status == UpdateStatus.appDownload)
          Expanded(
            child: LinearProgressIndicator(
              value: updateView.progress,
            ),
          ),
        const SizedBox(width: 16),
        if (updateView.status == UpdateStatus.appUpload || updateView.status == UpdateStatus.appDownload)
          Text("${(updateView.progress * 100).toStringAsFixed(2)}%"),
        if (updateView.status == UpdateStatus.appDone)
          Text("${(updateRestar - updateView.progress * 10).toStringAsFixed(0)} сек"),
      ],
    );
  }

  void showUpdateConfirmation(WidgetRef ref, FirmwareInfoModel frimwareInfo, BuildContext context, String oldVersion) {
    showDialog(
      context: context,
      builder: (BuildContext dialogContext) {
        return AlertDialog(
          title: const Text('Загрузка...'),
          content: Text('Текущая версия прошивки v$oldVersion, хотите загрузить версию v${frimwareInfo.version}?'),
          actions: [
            TextButton(
              child: const Text('Нет'),
              onPressed: () {
                Navigator.of(dialogContext).pop(false);

                ref.read(frimwareUpdateViewPrdr.notifier).update(btnAction: UpdateBtnAction.download);
              },
            ),
            TextButton(
              child: const Text('Да'),
              onPressed: () {
                Navigator.of(dialogContext).pop(true);

                ref.read(frimwareUpdateViewPrdr.notifier).update(btnAction: UpdateBtnAction.cancellation);

                downloadFirmwareIslyate(frimwareInfo);
              },
            ),
          ],
        );
      },
    );
  }
}
