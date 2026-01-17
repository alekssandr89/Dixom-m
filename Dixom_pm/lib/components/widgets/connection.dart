import 'package:flutter/material.dart';
import 'package:permission_handler/permission_handler.dart';

enum SettingsType { togle, slider, list }

const List<String> defList = [];

class SettingsChecked extends StatefulWidget {
  const SettingsChecked({
    super.key,
    required this.description,
    required this.parrametr,
    required this.state,
    required this.onTap,
    this.type = SettingsType.togle,
    this.list = defList,
  });
  final String description;
  final String parrametr;
  final bool state;
  final VoidCallback onTap;
  final SettingsType type;
  final List<String> list;

  @override
  SettingsCheckedState createState() => SettingsCheckedState();
}

class SettingsCheckedState extends State<SettingsChecked> {
  bool _switchValue = true;

  @override
  void initState() {
    super.initState();

    _switchValue = widget.state;
  }

  @override
  Widget build(BuildContext context) {
    if (widget.type == SettingsType.togle) {
      return Row(
        children: [
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  widget.parrametr,
                  style: const TextStyle(fontSize: 17, color: Colors.white),
                ),
                Text(
                  widget.description,
                  style: const TextStyle(fontSize: 16, color: Colors.white70),
                )
              ],
            ),
          ),
          const SizedBox(width: 16),
          Switch(
            value: _switchValue,
            onChanged: (newValue) {
              widget.onTap();
              setState(() {
                _switchValue = newValue;
              });
            },
          ),
        ],
      );
    } else if (widget.type == SettingsType.slider) {
      return Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text(
            widget.parrametr,
            style: const TextStyle(fontSize: 17, color: Colors.white),
          ),
          Text(
            widget.description,
            style: const TextStyle(fontSize: 16, color: Colors.white70),
          ),
          Row(
            children: [
              Text("0"),
              Expanded(
                child: Slider(
                  value: 0,
                  max: 100,
                  divisions: 5,
                  inactiveColor: Colors.white70,
                  onChanged: (double value) {},
                ),
              ),
            ],
          ),
        ],
      );
    } else if (widget.type == SettingsType.list) {
      return Text(
        widget.parrametr,
        style: const TextStyle(fontSize: 17, color: Colors.white),
      );
    } else {
      return Text(
        widget.parrametr,
        style: const TextStyle(fontSize: 17, color: Colors.white),
      );
    }
  }
}

class GettingPermision extends StatefulWidget {
  const GettingPermision({
    super.key,
    required this.description,
    required this.descriptionOk,
    required this.parrametr,
    required this.state,
    required this.permission,
  });
  final String descriptionOk;
  final String description;
  final String parrametr;
  final bool state;
  final Permission permission;

  @override
  GettingPermisionState createState() => GettingPermisionState();
}

class GettingPermisionState extends State<GettingPermision> {
  late final Permission permission;
  @override
  void initState() {
    permission = widget.permission;
    super.initState();
    _init();
  }

  bool permissionState = false;

  Future<void> _onButtonPressed() async {
    final isGranted = await permission.request().isGranted;
    setState(() {
      permissionState = isGranted;
    });
  }

  Future<void> _init() async {
    final isGranted = await permission.status;
    setState(() {
      permissionState = isGranted.isGranted;
    });
  }

  @override
  Widget build(BuildContext context) {
    final buttonColor = permissionState ? Colors.green : Colors.white;
    final description = permissionState ? widget.descriptionOk : widget.description;
    return Row(
      children: [
        Expanded(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                widget.parrametr,
                style: const TextStyle(fontSize: 17, color: Colors.white),
              ),
              Text(
                description,
                style: const TextStyle(fontSize: 16, color: Colors.white70),
              ),
            ],
          ),
        ),
        const SizedBox(width: 16),
        ElevatedButton(
          onPressed: _onButtonPressed,
          style: ElevatedButton.styleFrom(backgroundColor: buttonColor),
          child: Text(permissionState ? 'Разрешено' : 'Разрешить'),
        ),
      ],
    );
  }
}

class ButtoDescription extends StatelessWidget {
  const ButtoDescription(
      {super.key, required this.parrametr, required this.description, required this.onTap, required this.btnCaption});

  final String btnCaption;
  final String parrametr;
  final String description;
  final VoidCallback onTap;

  @override
  Widget build(BuildContext context) {
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
              ),
            ],
          ),
        ),
        const SizedBox(width: 16),
        ElevatedButton(
          onPressed: onTap,
          child: Text(btnCaption),
        ),
      ],
    );
  }
}
