import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../backend/filter.dart';
import '../../backend/sound.dart';

OverlayEntry overlayEntry = createOverlayEntry();
bool overlayState = false;

OverlayEntry createOverlayEntry() {
  TextEditingController channelNameController = TextEditingController();
  TextEditingController groupNameController = TextEditingController();

  return OverlayEntry(
    builder: (context) => Center(
      child: DecoratedBox(
        decoration: BoxDecoration(
          color: AppColor.allertBacground,
          borderRadius: BorderRadius.circular(10.0),
        ),
        child: Consumer(builder: (context, ref, child) {
          int currentChannel = ref.read(deviceSoundPrdr).currentChannel;
          String channelName = ref.read(soundEqProvider[currentChannel].select((value) => value.channelName));
          channelNameController.text = channelName;
          String groupName = ref.read(soundEqProvider[currentChannel].select((value) => value.groupName));
          groupNameController.text = groupName;
          int groupedChannel = ref.read(soundEqProvider[currentChannel]).groupChannel;
          bool isGrouped = ref.watch(deviceSoundPrdr).isGrouped[currentChannel ~/ 2];

          return SizedBox(
            width: 300,
            height: 220,
            child: Scaffold(
              backgroundColor: Colors.transparent,
              body: Column(
                children: [
                  const SizedBox(height: 10),
                  if (!isGrouped) Center(child: Text(" Настройки канала № ${currentChannel + 1}")),
                  if (isGrouped)
                    Center(child: Text("Настройка группы каналов №${currentChannel + 1} и №${groupedChannel + 1}")),
                  const SizedBox(height: 8.0),
                  if (!isGrouped)
                    Padding(
                      padding: const EdgeInsets.symmetric(horizontal: 16.0),
                      child: Row(
                        children: [
                          const Text("Имя канала:"),
                          const SizedBox(width: 16),
                          Expanded(
                            child: TextField(
                              controller: channelNameController,
                              onChanged: (String text) {
                                ref.read(soundEqProvider[currentChannel].notifier).updateChannelName(text);
                              },
                            ),
                          ),
                        ],
                      ),
                    ),
                  if (isGrouped)
                    Padding(
                      padding: const EdgeInsets.symmetric(horizontal: 16.0),
                      child: Row(
                        children: [
                          const Text("Имя группы:"),
                          const SizedBox(width: 16),
                          Expanded(
                            child: TextField(
                              controller: groupNameController,
                              onChanged: (String text) {
                                ref.read(soundEqProvider[currentChannel].notifier).updateGroupName(text);
                                ref.read(soundEqProvider[groupedChannel].notifier).updateGroupName(text);
                              },
                            ),
                          ),
                        ],
                      ),
                    ),
                  const SizedBox(height: 15.0),
                  SizedBox(
                    width: 260,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          ref.read(deviceSoundPrdr.notifier).updateGroup(currentChannel);
                        },
                        lable: isGrouped
                            ? "Разгруппировать каналы №${currentChannel + 1} и №${groupedChannel + 1}"
                            : "Сгруппировать с каналом №${groupedChannel + 1}",
                        lableSize: 13,
                        clickInfo: ClickInfo(state: isGrouped, type: TapType.tap),
                        mode: BtuttonMode.shine,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 20),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceAround,
                    children: [
                      SizedBox(
                        width: 110,
                        height: 40,
                        child: Button(
                            onChange: (ClickInfo info) {
                              overlayState = false;
                              overlayEntry.remove();
                            },
                            lable: "Не сохранять",
                            lableSize: 13,
                            mode: BtuttonMode.standart,
                            buttonOnColor: ColorControls.buttonBlue),
                      ),
                      SizedBox(
                        width: 110,
                        height: 40,
                        child: Button(
                            onChange: (ClickInfo info) {
                              ref.read(soundEqProvider[currentChannel].notifier).saveChannelName();
                              ref.read(soundEqProvider[currentChannel].notifier).saveGroupName();
                              ref.read(soundEqProvider[groupedChannel].notifier).saveChannelName();
                              ref.read(soundEqProvider[groupedChannel].notifier).saveGroupName();
                              overlayState = false;
                              overlayEntry.remove();
                            },
                            lable: "Сохранить",
                            lableSize: 13,
                            mode: BtuttonMode.standart,
                            buttonOnColor: ColorControls.buttonBlue),
                      ),
                    ],
                  ),
                ],
              ),
            ),
          );
        }),
      ),
    ),
  );
}
