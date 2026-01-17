import 'dart:io';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/dataBase/icons/custom_icons.dart';
import 'package:dixom/driver/android/service_channel.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/main.dart';
import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/driver/windows/usb_hid.dart';
import 'package:dixom/pages/top_bar/lower_menu.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class ConnectionStatus extends ConsumerWidget {
  const ConnectionStatus({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    //
    return Container(
      decoration: const BoxDecoration(
        color: AppColor.secondary,
        borderRadius: BorderRadius.all(Radius.circular(10)),
      ),
      child: Row(
        children: [
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                const Text("Состояние подключения", style: TextStyle(fontSize: 20, color: Colors.white70)),
                const Divider(),
                const SizedBox(height: 10),
                const InterfaceConnectionControl(),
                const SizedBox(height: 10),
                if (Platform.isAndroid) const Text("Bluetooth устройство"),
                if (Platform.isAndroid) const BlutoochDeviceSelect(),
                const SizedBox(height: 16),
                const InterfaceStatusInfo(),
                const SizedBox(height: AppIndents.pading),
                Text("Интерфейс обмена", style: TextStyle(fontSize: 20, color: Colors.white70)),
                Divider(),
                SizedBox(height: 10),
                Text("Интерфейс отправки данных на платформу", style: TextStyle(fontSize: 13, color: Colors.white70)),
                SizedBox(height: 2),
                TranmitInterfaceControl(),
                SizedBox(height: 10),
                ResieverInterfaceControl(),
                SizedBox(height: 20),
              ],
            ),
          ),
          // SizedBox(width: 26),
        ],
      ),
    );
  }
}

class BlutoochDeviceSelect extends StatefulWidget {
  const BlutoochDeviceSelect({
    super.key,
  });

  @override
  State<BlutoochDeviceSelect> createState() => _BlutoochDeviceSelectState();
}

class _BlutoochDeviceSelectState extends State<BlutoochDeviceSelect> {
  final List<String> items = ["Нет данных"];
  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        Expanded(
          flex: 4,
          child: DropdownButton<String>(
            isExpanded: true,
            hint: const Text('Нет доступных устройств'),
            value: items[0].isNotEmpty ? items[0] : "Пусто",
            onChanged: (String? newValue) async {
              /// setBluetootDevice(newValue is String ? newValue : "Dixom Hi-Res Audio");
              String? name = await sendStringToAndroid(methodSetBluetoothDevice,
                  param: newValue is String ? newValue : "Dixom Hi-Res Audio");

              if (name is String) {
                appRef.read(connectionState.notifier).copyWith(selectedBluetoothDevice: name);
              }
            },
            items: items.map<DropdownMenuItem<String>>((String value) {
              return DropdownMenuItem<String>(
                value: value,
                child: Text(value),
              );
            }).toList(),
          ),
        ),
        const SizedBox(width: 16),
        Expanded(
          flex: 1,
          child: ElevatedButton(
            onPressed: () async {
              if (Platform.isWindows) {}
              if (Platform.isAndroid) {
                List<String> btDeviceList = await getBluetootDeviceList();
                items.clear();
                for (int i = 0; i < btDeviceList.length; i++) {
                  items.add(btDeviceList[i]);
                }
                setState(() {});
              }
            },
            child: const Icon(CustomIcons.sync_icon),
          ),
        ),
      ],
    );
  }
}

class TranmitInterfaceControl extends StatelessWidget {
  const TranmitInterfaceControl({
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        Expanded(
          child: Consumer(builder: (context, ref, child) {
            final connInfo = ref.watch(connectionState);

            return SegmentedButton<ExchangeInterface>(
              segments: <ButtonSegment<ExchangeInterface>>[
                ButtonSegment<ExchangeInterface>(
                    value: ExchangeInterface.usbHid,
                    label: const Text('HID'),
                    enabled: connInfo.statusUsbHid == WorkState.connected ? true : false,
                    icon: const Icon(CustomIcons.usb)),
                ButtonSegment<ExchangeInterface>(
                    value: ExchangeInterface.usbCdc,
                    label: const Text('CDC'),
                    enabled: connInfo.statusUsbCdc == WorkState.connected ? true : false,
                    icon: const Icon(CustomIcons.usb)),
                ButtonSegment<ExchangeInterface>(
                    value: ExchangeInterface.bluetoothSpp,
                    enabled: connInfo.statusBluetooth == WorkState.connected ? true : false,
                    label: const Text('Bluetooth'),
                    icon: const Icon(CustomIcons.bluetooth)),
              ],
              selected: <ExchangeInterface>{connInfo.flutterTx},
              onSelectionChanged: (Set<ExchangeInterface> newSelection) {
                setTransmitInterface(newSelection.first);
              },
            );
          }),
        ),
      ],
    );
  }
}

class InterfaceStatusInfo extends StatelessWidget {
  const InterfaceStatusInfo({
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    return Table(
      border: TableBorder.all(color: Colors.grey, borderRadius: const BorderRadius.all(Radius.circular(10))),
      columnWidths: const {
        0: FixedColumnWidth(100),
      },
      children: [
        TableRow(
          decoration: const BoxDecoration(),
          children: [
            statusTableTextHeader("Интерфейс"),
            statusTableTextHeader("Чтение"),
            statusTableTextHeader("Ошибка"),
            statusTableTextHeader("Запись"),
            statusTableTextHeader("Ошибка"),
          ],
        ),
        TableRow(children: exchangeInfo(usbHidProvider, ExchangeInterface.usbHid)),
        //  TableRow(children: exchangeInfo(usbCdcProvider, ExchangeInterface.usbCdc)),
        if (Platform.isAndroid) TableRow(children: exchangeInfo(bluetoothSppProvider, ExchangeInterface.bluetoothSpp)),
      ],
    );
  }
}

class InterfaceConnectionControl extends StatelessWidget {
  const InterfaceConnectionControl({
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    return Table(
      border: TableBorder.all(color: Colors.grey, borderRadius: const BorderRadius.all(Radius.circular(10))),
      columnWidths: const {
        0: FixedColumnWidth(100),
        1: FixedColumnWidth(160),
      },
      defaultVerticalAlignment: TableCellVerticalAlignment.middle,
      children: [
        TableRow(decoration: const BoxDecoration(), children: [
          statusTableTextHeader("Интерфейс"),
          statusTableTextHeader("Подключение"),
          statusTableTextHeader("Состояние"),
        ]),
        TableRow(children: intrfaceState(usbHidProvider, ExchangeInterface.usbHid)),
        //  TableRow(children: intrfaceState(usbCdcProvider, ExchangeInterface.usbCdc)),
        if (Platform.isAndroid) TableRow(children: intrfaceState(bluetoothSppProvider, ExchangeInterface.bluetoothSpp)),
      ],
    );
  }
}

class ResieverInterfaceControl extends StatelessWidget {
  const ResieverInterfaceControl({
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    return Consumer(
      builder: (context, ref, child) {
        final interfaceState = ref.watch(connectionState);

        Set<ExchangeInterface> selection = <ExchangeInterface>{};

        if (interfaceState.manualTxHid) {
          selection.add(ExchangeInterface.usbHid);
        }
        if (interfaceState.manualTxCdc) {
          selection.add(ExchangeInterface.usbCdc);
        }
        if (interfaceState.manualTxBluetooth) {
          selection.add(ExchangeInterface.bluetoothSpp);
        }

        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
                "Интерфейс получения данных из платформы. Интерфейс будет выбираться автоматический в зависимости от того с какого интерфейса поступили данные",
                style: TextStyle(fontSize: 13, color: Colors.white70)),
            const SizedBox(height: 2),
            Row(
              children: [
                Expanded(
                  child: SegmentedButton<ExchangeInterface>(
                    segments: <ButtonSegment<ExchangeInterface>>[
                      ButtonSegment<ExchangeInterface>(
                          value: ExchangeInterface.usbHid,
                          label: const Text('HID'),
                          enabled: interfaceState.connectionHid,
                          icon: const Icon(CustomIcons.usb)),
                      ButtonSegment<ExchangeInterface>(
                          value: ExchangeInterface.usbCdc,
                          label: const Text('CDC'),
                          enabled: interfaceState.connectionCdc,
                          icon: const Icon(CustomIcons.usb)),
                      ButtonSegment<ExchangeInterface>(
                          value: ExchangeInterface.bluetoothSpp,
                          enabled: interfaceState.connectionBluetooth,
                          label: const Text('Bluetooth'),
                          icon: const Icon(CustomIcons.bluetooth)),
                    ],
                    selected: <ExchangeInterface>{interfaceState.flutterRx},
                    onSelectionChanged: (Set<ExchangeInterface> newSelection) {
                      ExchangeInterface selectedInterface = newSelection.first;
                      String inteface = selectedInterface.index.toString();

                      String hid = interfaceState.manualTxHid ? "1" : "0";
                      String cdc = interfaceState.manualTxCdc ? "1" : "0";
                      String bluetooth = interfaceState.manualTxBluetooth ? "1" : "0";

                      transmitter("0 5=$inteface $hid $cdc $bluetooth");
                    },
                  ),
                ),
              ],
            ),
            const SizedBox(height: 10),
            const Text("Выберите интерфейс в который необходимо отправлять данные не зависимо от выбранного онтерфеса",
                style: TextStyle(fontSize: 13, color: Colors.white70)),
            const SizedBox(height: 2),
            Row(
              children: [
                Expanded(
                  child: SegmentedButton<ExchangeInterface>(
                    multiSelectionEnabled: true,
                    emptySelectionAllowed: true,
                    segments: <ButtonSegment<ExchangeInterface>>[
                      ButtonSegment<ExchangeInterface>(
                          value: ExchangeInterface.usbHid,
                          label: const Text('HID'),
                          enabled: interfaceState.connectionHid,
                          icon: const Icon(CustomIcons.usb)),
                      ButtonSegment<ExchangeInterface>(
                          value: ExchangeInterface.usbCdc,
                          label: const Text('CDC'),
                          enabled: interfaceState.connectionCdc,
                          icon: const Icon(CustomIcons.usb)),
                      ButtonSegment<ExchangeInterface>(
                          value: ExchangeInterface.bluetoothSpp,
                          enabled: interfaceState.connectionBluetooth,
                          label: const Text('Bluetooth'),
                          icon: const Icon(CustomIcons.bluetooth)),
                    ],
                    selected: selection,
                    onSelectionChanged: (Set<ExchangeInterface> newSelection) {
                      String hid = newSelection.contains(ExchangeInterface.usbHid) ? "1" : "0";
                      String cdc = newSelection.contains(ExchangeInterface.usbCdc) ? "1" : "0";
                      String bluetooth = newSelection.contains(ExchangeInterface.bluetoothSpp) ? "1" : "0";

                      String inteface = interfaceState.flutterRx.index.toString();

                      transmitter("0 5=$inteface $hid $cdc $bluetooth");
                    },
                  ),
                ),
              ],
            ),
          ],
        );
      },
    );
  }
}

List<Widget> intrfaceState(StateNotifierProvider<InterfaceConnectionNotifier, InterfaceConnectionModel> provider,
    ExchangeInterface interface) {
  List<Widget> param = [
    statusTableTextParam(interface.decoding),
    Consumer(
      builder: (context, ref, child) {
        final interfaceState = ref.watch(provider.select((value) => value.status));

        bool btnState = true;
        String bluetoothDevice = "";

        if (interface == ExchangeInterface.usbHid) {
          btnState = ref.watch(connectionState.select((value) => value.autoConnectionHid));
        } else if (interface == ExchangeInterface.usbCdc) {
          btnState = ref.watch(connectionState.select((value) => value.autoConnectionCdc));
        } else if (interface == ExchangeInterface.bluetoothSpp) {
          btnState = ref.watch(connectionState.select((value) => value.autoConnectionBluetooth));
          bluetoothDevice = ref.watch(connectionState.select((value) => value.selectedBluetoothDevice));
        }

        String btnName = "Отключиться";
        if (interfaceState != WorkState.connected) {
          btnName = "Подключиться";
        }
        if (btnState == true) {
          btnName = "Авто подключение";
        }

        if (interface == ExchangeInterface.bluetoothSpp) {
          if (interfaceState != WorkState.connected) {
            btnName = "$btnName к $bluetoothDevice";
          } else {
            btnName = "$btnName от $bluetoothDevice";
          }
        }

        return Column(
          children: [
            Padding(
              padding: const EdgeInsets.all(4.0),
              child: ElevatedButton(
                onPressed: !btnState
                    ? () {
                        //Ручное подключение к интерфейсам
                        if (interface == ExchangeInterface.usbHid) {
                          if (interfaceState == WorkState.connected) {
                            if (Platform.isWindows) {
                              disconnectUsb();
                            }
                            if (Platform.isAndroid) {
                              AndroidService.exchange(
                                  "$methodSetConnection ${ExchangeInterface.usbHid.index} ${WorkState.disconnected.index}");
                            }
                          } else {
                            if (Platform.isWindows) {
                              connectUsb();
                            }
                            if (Platform.isAndroid) {
                              AndroidService.exchange(
                                  "$methodSetConnection ${ExchangeInterface.usbHid.index} ${WorkState.connected.index}");
                            }
                          }
                        } else if (interface == ExchangeInterface.usbCdc) {
                          if (interfaceState == WorkState.connected) {
                            if (Platform.isWindows) {}
                            if (Platform.isAndroid) {
                              AndroidService.exchange(
                                  "$methodSetConnection ${ExchangeInterface.usbCdc.index} ${WorkState.disconnected.index}");
                            }
                          } else {
                            if (Platform.isWindows) {}
                            if (Platform.isAndroid) {
                              AndroidService.exchange(
                                  "$methodSetConnection ${ExchangeInterface.usbCdc.index} ${WorkState.connected.index}");
                            }
                          }
                        } else if (interface == ExchangeInterface.bluetoothSpp) {
                          if (interfaceState == WorkState.connected) {
                            if (Platform.isWindows) {}
                            if (Platform.isAndroid) {
                              AndroidService.exchange(
                                  "$methodSetConnection ${ExchangeInterface.bluetoothSpp.index} ${WorkState.disconnected.index}");
                            }
                          } else {
                            if (Platform.isWindows) {}
                            if (Platform.isAndroid) {
                              AndroidService.exchange(
                                  "$methodSetConnection ${ExchangeInterface.bluetoothSpp.index} ${WorkState.connected.index}");
                            }
                          }
                        }
                      }
                    : null,
                child: Text(btnName, textAlign: TextAlign.center),
              ),
            ),
          ],
        );
      },
    ),
    Consumer(
      builder: (context, ref, child) {
        final WorkState value = ref.watch(provider.select((value) => value.status));
        return intrfaceStateDecode(value);
      },
    ),
  ];
  return param;
}

Widget intrfaceStateDecode(WorkState value) {
  Color color;
  if (value == WorkState.connected) {
    color = const Color.fromARGB(255, 52, 104, 47);
  } else if (value == WorkState.disconnected) {
    color = const Color.fromARGB(255, 100, 42, 61);
  } else if (value == WorkState.notFound || value == WorkState.notAvailable) {
    color = const Color.fromARGB(255, 54, 54, 27);
  } else if (value == WorkState.connecting) {
    color = const Color.fromARGB(255, 42, 72, 100);
  } else if (value == WorkState.error) {
    color = const Color.fromARGB(255, 131, 0, 0);
  } else {
    color = AppColor.secondary;
  }
  return Container(
    color: color,
    padding: const EdgeInsets.all(6),
    child: Text("Код ${value.index} ${value.decoding}"),
  );
}

List<Widget> exchangeInfo(StateNotifierProvider<InterfaceConnectionNotifier, InterfaceConnectionModel> provider,
    ExchangeInterface interface) {
  return [
    statusTableTextParam(interface.decoding),
    getStatusTableTextParams(InterfaceState.succesIn, provider),
    getStatusTableTextParams(InterfaceState.errorIn, provider),
    getStatusTableTextParams(InterfaceState.succesOut, provider),
    getStatusTableTextParams(InterfaceState.errorOut, provider),
  ];
}

Consumer getStatusTableTextParams(
    InterfaceState select, StateNotifierProvider<InterfaceConnectionNotifier, InterfaceConnectionModel> provider) {
  return Consumer(
    builder: (context, ref, child) {
      final value = ref.watch(provider.select((value) {
        switch (select) {
          case InterfaceState.succesIn:
            return value.succesIn;
          case InterfaceState.errorIn:
            return value.errorIn;
          case InterfaceState.succesOut:
            return value.succesOut;
          case InterfaceState.errorOut:
            return value.errorOut;
          default:
            return value.succesIn;
        }
      }));
      return statusTableTextParam(value.toString());
    },
  );
}
