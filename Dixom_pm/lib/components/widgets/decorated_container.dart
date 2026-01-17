import 'package:flutter/material.dart';

import '../../dataBase/app_constants.dart';

class DecoratedContainer extends StatelessWidget {
  const DecoratedContainer({
    super.key,
    required this.child,
    this.top = AppIndents.pading,
    this.left = AppIndents.pading,
    this.right = AppIndents.pading,
    this.bottom = AppIndents.pading,
    this.color = AppColor.secondary,
  });

  final Widget child;
  final double left, top, right, bottom;
  final Color color;

  @override
  Widget build(BuildContext context) {
    return Container(
      margin: EdgeInsets.only(
        top: top,
        left: left,
        right: right,
        bottom: bottom,
      ),
      padding: const EdgeInsets.all(AppIndents.pading),
      decoration: BoxDecoration(
        color: color,
        borderRadius: const BorderRadius.all(Radius.circular(10)),
      ),
      child: child,
    );
  }
}
