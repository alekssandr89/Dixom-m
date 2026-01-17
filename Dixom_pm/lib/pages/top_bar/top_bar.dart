import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/dataBase/icons/custom_icons.dart';
import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/dataBase/pages.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter/material.dart';

import 'package:flutter_riverpod/flutter_riverpod.dart';

class DixomAppBar extends ConsumerWidget {
  final bool backState;
  final bool connState;
  static bool intitState = false;

  const DixomAppBar({
    super.key,
    this.backState = true,
    this.connState = true,
  });

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return Padding(
      padding: const EdgeInsets.only(left: AppIndents.pading, top: 0, right: AppIndents.pading),
      child: Column(
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              if (!Responsive.isDesktop(context) && backState == false)
                IconButton(
                  onPressed: () {
                    // context.read<MenuAppController>().controlMenu;
                    Scaffold.of(context).openDrawer();
                  },
                  icon: const Icon(Icons.menu, color: Colors.white70),
                ),
              if (Responsive.isDesktop(context))
                const SizedBox(
                  height: 40,
                ),
              if (backState == true)
                IconButton(
                    onPressed: () {
                      Navigator.pop(context);
                    },
                    icon: const Icon(Icons.keyboard_backspace, color: Colors.white70)),

              if (!Responsive.isMobile(context)) const Text("Аудиопроцессор", style: TextStyle(fontSize: 15)),
              if (!Responsive.isMobile(context)) Spacer(flex: Responsive.isDesktop(context) ? 2 : 1),
              //  const Expanded(child: VolumeControl()),
              //  const SourceControl(),

              InkWell(
                onTap: () {
                  // действия при нажатии на строку
                  if (connState) {
                    Navigator.pushNamed(context, AppPage.connection);
                  }
                },
                child: Consumer(
                  builder: (context, ref, child) {
                    final WorkState state = ref.watch(connectionState.select((value) => value.status));
                    final ExchangeInterface txInterface = ref.watch(connectionState.select((value) => value.flutterRx));
                    final ExchangeInterface rxInterface = ref.watch(connectionState.select((value) => value.flutterTx));
                    String connectionText = "Связь не установлена";
                    IconData connectionIcon = CustomIcons.cancel_circle;
                    Color connectionColor = AppColor.error;

                    if (state == WorkState.connected) {
                      connectionText = "Подключено: ⇧ ${rxInterface.decoding} ⇩ ${txInterface.decoding}";
                      connectionIcon = CustomIcons.ok_circle;
                      connectionColor = AppColor.ok;
                    }
                    return Row(
                      children: [
                        Icon(connectionIcon, color: connectionColor, size: 17),
                        const SizedBox(width: 5),
                        Text(connectionText),
                      ],
                    );
                  },
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }
}
