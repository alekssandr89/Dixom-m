import 'package:flutter/material.dart';

Future<bool> confirmationDialog(BuildContext context,
    {String title = "Сброс", String description = ""}) async {
  bool answer = false;

  await showDialog(
    context: context,
    builder: (BuildContext context) {
      return AlertDialog(
        title: Text(title),
        content: Text(description),
        actions: <Widget>[
          TextButton(
            child: const Text('Отмена'),
            onPressed: () {
              Navigator.of(context).pop();
              answer = false;
            },
          ),
          TextButton(
            child: const Text('Ок'),
            onPressed: () {
              Navigator.of(context).pop();
              answer = true;
            },
          ),
        ],
      );
    },
  );

  return answer;
}
