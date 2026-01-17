import 'package:dixom/dataBase/app_constants.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../backend/can_model.dart';
import '../backend/can_provider.dart';

void canFilter(BuildContext context) {
  showDialog(
    context: context,
    builder: (BuildContext context) {
      return AlertDialog(
        backgroundColor: ColorPanel.background,
        title: const Text("Фильтр can шины"),
        content: const SizedBox(height: 300, width: 300, child: CanFilter()),
        actions: <Widget>[
          TextButton(
            child: const Text('Закрыть'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          ),
        ],
      );
    },
  );
}

class CanFilter extends StatefulWidget {
  const CanFilter({super.key});

  @override
  State<CanFilter> createState() => _CanSetupState();
}

class _CanSetupState extends State<CanFilter> {
  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Consumer(builder: (context, ref, child) {
      CanStateModel canState = ref.watch(canStatePrdr);
      return const Column(
        children: [
          Text("Фильтры в разработке"),
          /*SizedBox(
            width: 150,
            height: 35,
            child: Button(
                onChange: (ClickInfo info) {
                  ref.read(canStatePrdr.notifier).setTxPermision();
                },
                lable: "Только чтение",
                lableSize: 13,
                clickInfo: ClickInfo(state: canState.txPermision),
                mode: BtuttonMode.shine,
                buttonOnColor: ColorControls.buttonBlue),
          ),*/
          SizedBox(height: 16),
        ],
      );
    });
  }
}
