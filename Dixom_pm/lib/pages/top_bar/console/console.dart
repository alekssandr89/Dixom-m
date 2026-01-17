import 'dart:async';
import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/pages/top_bar/console/quick_commands.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:easy_localization/easy_localization.dart';
import 'package:flutter/material.dart';
import 'package:syncfusion_flutter_datagrid/datagrid.dart';

import '../../../dataBase/app_constants.dart';
import '../../../generated/locale_keys.g.dart';

Widget appConsole(BuildContext context) {
  return Row(
    crossAxisAlignment: CrossAxisAlignment.start,
    children: [
      if (!Responsive.isMobile(context)) const QuickCommands(),
      DataGrid(),
    ],
  );
}

class DataGrid extends StatefulWidget {
  const DataGrid({super.key});

  @override
  DataGridState createState() => DataGridState();
}

class DataGridState extends State<DataGrid> {
  late ExchangeDataSource exchangeDataSource;

  @override
  void initState() {
    super.initState();
    exchangeDataSource = ExchangeDataSource();
    _dataGridController.scrollToRow(exchangeDataSource.rows.length - 1);
  }

  @override
  void dispose() {
    super.dispose();
  }

  final _dataGridController = DataGridController();
  final TextEditingController _controller = TextEditingController();

  @override
  Widget build(BuildContext context) {
    return Expanded(
      flex: 3,
      child: Container(
        padding: const EdgeInsets.all(AppIndents.pading),
        // margin: const EdgeInsets.only(bottom: AppIndents.pading),
        decoration: const BoxDecoration(
          color: AppColor.secondary,
          // borderRadius: BorderRadius.all(Radius.circular(10)),
        ),
        child: Column(
          children: [
            if (Responsive.isMobile(context)) const Expanded(child: QuickCommands()),
            Expanded(
              flex: 2,
              child: StreamBuilder<int>(
                stream: DataConsole.console.stream,
                builder: (context, snapshot) {
                  exchangeDataSource = ExchangeDataSource();
                  WidgetsBinding.instance.addPostFrameCallback((_) {
                    _dataGridController.scrollToRow(exchangeDataSource.rows.length - 1);
                  });
                  return SfDataGrid(
                    rowHeight: 40,
                    headerRowHeight: 35,
                    selectionMode: SelectionMode.single,
                    source: exchangeDataSource,
                    gridLinesVisibility: GridLinesVisibility.both,
                    headerGridLinesVisibility: GridLinesVisibility.both,
                    controller: _dataGridController,
                    columns: _getColumns(),
                    columnWidthMode: ColumnWidthMode.lastColumnFill,
                    showSortNumbers: true,
                  );
                },
              ),
            ),
            const SizedBox(width: AppIndents.pading),
            commandSend(),
            //    button(),
          ],
        ),
      ),
    );
  }

  Row commandSend() {
    return Row(
      children: [
        Expanded(
          child: TextField(
            controller: _controller,
            decoration: const InputDecoration(
              hintText: 'Введите команду здесь',
            ),
          ),
        ),
        const SizedBox(width: AppIndents.pading),
        ElevatedButton(
          onPressed: () {
            transmitter(_controller.text);
          },
          child: const Text('>'),
        ),
        const SizedBox(width: AppIndents.pading),
        ElevatedButton(
          onPressed: () {
            dataConsole.clearAll();
          },
          child: const Text('Очистить'),
        ),
      ],
    );
  }
}

DataConsole dataConsole = DataConsole();

class DataConsole {
  static int exchangeDataMaxLen = 100;
  static List<CommandMsg> exchangeData = [];
  static ConsoleController console = ConsoleController();

  void clearAll() {
    exchangeData.clear();
    console.uiNotify(0);
  }

  void addMessage(CommandMsg cmdMsg) {
    if (cmdMsg.showInConsole) {
      if (exchangeData.length > exchangeDataMaxLen) {
        exchangeData.removeAt(0);
      }
      exchangeData.add(cmdMsg);
      console.uiNotify(0);
    }
  }
}

class ConsoleController {
  StreamController<int> msg = StreamController<int>.broadcast(sync: false);
  Stream<int> get stream => msg.stream;

  void uiNotify(int data) {
    msg.sink.add(data);
  }

  void dispose() {
    msg.close();
  }
}

const String consoleTime = "time";
const String consoleData = "data";
const String consoleConnection = "connection";
const String consoleDirection = "direction";

List<GridColumn> _getColumns() {
  return <GridColumn>[
    GridColumn(
      columnName: consoleConnection,
      label: Container(
        // padding: const EdgeInsets.all(16.0),
        //color: Color.fromARGB(255, 22, 51, 68),
        alignment: Alignment.center,
        child: Text(
          LocaleKeys.consoleConnection.tr(),
        ),
      ),
    ),
    GridColumn(
      columnWidthMode: ColumnWidthMode.fill,
      columnName: consoleData,
      label: Container(
        // color: Color.fromARGB(255, 33, 18, 61),
        alignment: Alignment.center,
        child: Text(
          LocaleKeys.consoleData.tr(),
          overflow: TextOverflow.ellipsis,
        ),
      ),
    ),
  ];
}

/// An object to set the employee collection data source to the datagrid. This
/// is used to map the employee data to the datagrid widget.
class ExchangeDataSource extends DataGridSource {
  /// Creates the employee data source class with required details.
  ///
  ///
  late List<CommandMsg> _exchangeData;

  ExchangeDataSource() {
    _exchangeData = DataConsole.exchangeData;
    updateDataGridRows();
  }

  List<DataGridRow> dataGridRow = [];

  void updateDataGridRows() {
    dataGridRow = _exchangeData
        .map<DataGridRow>((e) => DataGridRow(cells: [
              DataGridCell<String>(columnName: consoleTime, value: e.interface.decoding),
              /*DataGridCell<String>(
                  columnName: consoleTime, value: e.direction.decoding),
              DataGridCell<String>(columnName: consoleTime, value: e.time),*/
              DataGridCell<String>(columnName: consoleData, value: e.dataUtf8),
            ]))
        .toList();
  }

  void updateDataGridSource() {
    notifyListeners();
  }

  @override
  List<DataGridRow> get rows => dataGridRow;

  @override
  DataGridRowAdapter buildRow(DataGridRow row) {
    return DataGridRowAdapter(
        cells: row.getCells().map<Widget>((e) {
      return Container(
        //  alignment: Alignment.center,
        padding: const EdgeInsets.all(8.0),
        child: Text(e.value.toString()),
      );
    }).toList());
  }
}
