import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:easy_localization/easy_localization.dart';
import 'package:flutter/material.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'backend/can_model.dart';
import 'backend/can_provider.dart';
import 'backend/can_reciever.dart';
import 'view/can_filter.dart';
import 'view/can_settings.dart';
import 'package:data_table_2/data_table_2.dart';

class CanBusPage extends StatelessWidget {
  const CanBusPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      drawer: const Menu(),
      body: SafeArea(
        child: Container(
          color: ColorPanel.background,
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              if (Responsive.isDesktop(context)) const Expanded(child: Menu()),
              Expanded(
                flex: 4,
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.start,
                  children: [
                    const DixomAppBar(backState: false),
                    Expanded(
                      child: Container(
                        padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 20),
                        child: Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            Expanded(
                              child: Consumer(builder: (context, ref, child) {
                                ref.watch(canFramePrdr);
                                if (canMessageMap.isEmpty) {
                                  return const Text("Данных нет...");
                                } else {
                                  return DataTable2(
                                    columns: const [
                                      // DataColumn(label: Text('Type')),
                                      DataColumn2(
                                        label: Text('ID'),
                                        size: ColumnSize.S,
                                      ),
                                      DataColumn2(
                                        label: Text('DLC'),
                                        size: ColumnSize.S,
                                      ),
                                      DataColumn2(
                                        label: Text('Count'),
                                        size: ColumnSize.S,
                                      ),
                                      DataColumn2(
                                        label: Text('Data'),
                                        size: ColumnSize.L,
                                      ),
                                      DataColumn2(
                                        label: Text('Period'),
                                        size: ColumnSize.S,
                                      ),
                                      DataColumn2(
                                        label: Text('Time'),
                                        size: ColumnSize.S,
                                      ),
                                    ],
                                    rows: canMessageMap.values.map((message) {
                                      return DataRow(cells: [
                                        //   DataCell(Text(message.idType.toString())),
                                        DataCell(Text(message.id.toString())),
                                        DataCell(Text(message.dlc.toString())),
                                        DataCell(Text(message.framesCount.toString())),
                                        DataCell(Text(message.buffer
                                            .map((byte) => byte.toRadixString(16).padLeft(2, '0'))
                                            .join(' '))),
                                        DataCell(Text(message.period.inMilliseconds.toString())),
                                        DataCell(Text(DateFormat('mm:ss.SSS').format(message.time))),
                                      ]);
                                    }).toList(),
                                  );
                                }
                              }),
                            ),
                            const SizedBox(height: 16),
                            Consumer(builder: (context, ref, child) {
                              CanStateModel status = ref.watch(canStatePrdr);
                              return SingleChildScrollView(
                                scrollDirection: Axis.horizontal,
                                child: Row(
                                  children: [
                                    SizedBox(
                                      width: 150,
                                      height: 35,
                                      child: Button(
                                          onChange: (ClickInfo info) {
                                            ref.read(canStatePrdr.notifier).connecting();
                                          },
                                          lable: status.status == WorkState.connected ? "Отключиться" : "Подключиться",
                                          lableSize: 13,
                                          clickInfo:
                                              ClickInfo(state: status.status == WorkState.connected ? true : false),
                                          mode: BtuttonMode.shine,
                                          buttonOnColor: ColorControls.buttonBlue),
                                    ),
                                    const SizedBox(width: 16),
                                    SizedBox(
                                      width: 150,
                                      height: 35,
                                      child: Button(
                                          onChange: (ClickInfo info) {
                                            ref.read(canStatePrdr.notifier).setDataOutPermision();
                                          },
                                          lable: "Приём данных",
                                          lableSize: 13,
                                          clickInfo: ClickInfo(state: status.dataOutPermision),
                                          mode: BtuttonMode.shine,
                                          buttonOnColor: ColorControls.buttonBlue),
                                    ),
                                    const SizedBox(width: 16),
                                    SizedBox(
                                      width: 110,
                                      height: 35,
                                      child: Button(
                                          onChange: (ClickInfo info) {
                                            canSetup(context);
                                          },
                                          lable: "Настройки",
                                          lableSize: 13,
                                          mode: BtuttonMode.standart,
                                          buttonOnColor: ColorControls.buttonBlue),
                                    ),
                                    const SizedBox(width: 16),
                                    SizedBox(
                                      width: 110,
                                      height: 35,
                                      child: Button(
                                          onChange: (ClickInfo info) {
                                            canFilter(context);
                                          },
                                          lable: "Can фильтр",
                                          lableSize: 13,
                                          mode: BtuttonMode.standart,
                                          buttonOnColor: ColorControls.buttonBlue),
                                    ),
                                  ],
                                ),
                              );
                            }),
                          ],
                        ),
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
