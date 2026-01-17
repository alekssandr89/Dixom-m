import 'package:flutter/material.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/components/utilities/responsive.dart';

class MyCarPage extends StatelessWidget {
  const MyCarPage({super.key});

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
                        child: const Text("Првиет "),
                      ),
                    )
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
