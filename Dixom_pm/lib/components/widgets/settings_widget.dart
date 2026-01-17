import 'package:flutter/material.dart';

Row settingsTogle(
    {required String description,
    required String parrametr,
    required bool state,
    required ValueChanged<bool> onChange}) {
  return Row(
    children: [
      Expanded(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              parrametr,
              style: const TextStyle(fontSize: 17, color: Colors.white),
            ),
            Text(
              description,
              style: const TextStyle(fontSize: 16, color: Colors.white70),
            )
          ],
        ),
      ),
      const SizedBox(width: 16),
      Switch(
        value: state,
        onChanged: (newValue) {
          onChange(newValue);
        },
      ),
      const SizedBox(width: 8),
    ],
  );
}

Column settingsSlider({
  required String description,
  required String parrametr,
  required double value,
  required ValueChanged<double> onChange,
  double valueMin = 0,
  String measure = "",
  int delimiter = 0,
  required double valueMax,
}) {
  return Column(
    crossAxisAlignment: CrossAxisAlignment.start,
    children: [
      Text(
        parrametr,
        style: const TextStyle(fontSize: 17, color: Colors.white),
      ),
      Text(
        description,
        style: const TextStyle(fontSize: 16, color: Colors.white70),
      ),
      Row(
        children: [
          Text("${delimiter == 0 ? value.toInt() : delimiter == 1 ? value / 10 : value / 100} $measure"),
          Expanded(
            child: Slider(
              value: value,
              min: valueMin,
              max: valueMax,
              divisions: (valueMax - valueMin).toInt().ceil(),
              onChanged: (double value) {
                onChange(value);
              },
            ),
          ),
        ],
      ),
    ],
  );
}

Column settingsList({
  required String description,
  required String parrametr,
  required int value,
  required final List<String> list,
  required ValueChanged<int> onChange,
}) {
  return Column(
    crossAxisAlignment: CrossAxisAlignment.start,
    children: [
      Text(
        parrametr,
        style: const TextStyle(fontSize: 17, color: Colors.white),
      ),
      Text(
        description,
        style: const TextStyle(fontSize: 16, color: Colors.white70),
      ),
      Row(
        children: [
          Expanded(
              child: DropdownButton<int>(
            value: value,
            onChanged: (newValue) {
              onChange(newValue!);
            },
            items: List<DropdownMenuItem<int>>.generate(list.length, (index) {
              return DropdownMenuItem<int>(
                value: index,
                child: Text(list[index]),
              );
            }),
          )),
        ],
      ),
    ],
  );
}
