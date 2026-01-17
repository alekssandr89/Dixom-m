import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/dataBase/icons/custom_icons.dart';
import 'package:dixom/pages/top_bar/connection/connection.dart';
import 'package:dixom/pages/top_bar/console/console.dart';
import 'package:dixom/pages/top_bar/log/view/device_log.dart';
import 'package:dixom/pages/top_bar/device_info/view/information.dart';
import 'package:dixom/pages/top_bar/register/view/register.dart';
import 'package:dixom/pages/top_bar/update/view/update_view.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:flutter/material.dart';

class MainConnectionPage extends StatefulWidget {
  const MainConnectionPage({super.key});

  @override
  MainConnectionPageState createState() => MainConnectionPageState();
}

class MainConnectionPageState extends State<MainConnectionPage> {
  static int _selectedIndex = 2;

  final PageController _pageController = PageController(initialPage: _selectedIndex);

  void _onPageChanged(int index) {
    setState(() {
      _selectedIndex = index;
    });
  }

  void _onBottomNavTap(int index) {
    _pageController.animateToPage(index, duration: const Duration(milliseconds: 300), curve: Curves.easeInOut);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      bottomNavigationBar: ConBottomNavigation(
        //currentIndex: _selectedIndex,
        setIndex: _selectedIndex,
        onPressed: _onBottomNavTap,
      ),
      body: SafeArea(
        child: Column(
          children: [
            const DixomAppBar(
              connState: false,
            ),
            Expanded(
              child: GestureDetector(
                onHorizontalDragEnd: (details) {
                  if (details.velocity.pixelsPerSecond.dx > 0) {
                    // переключение на предыдущую страницу
                    int newIndex = _selectedIndex - 1;
                    if (newIndex >= 0) {
                      _pageController.animateToPage(newIndex,
                          duration: const Duration(milliseconds: 300), curve: Curves.easeInOut);
                      _onPageChanged(newIndex);
                    }
                  } else {
                    // переключение на следующую страницу
                    int newIndex = _selectedIndex + 1;
                    if (newIndex < 6) {
                      _pageController.animateToPage(newIndex,
                          duration: const Duration(milliseconds: 300), curve: Curves.easeInOut);
                      _onPageChanged(newIndex);
                    }
                  }
                },
                child: PageView(
                  controller: _pageController,
                  onPageChanged: _onPageChanged,
                  children: [
                    const FirmwareUpdateView(),
                    const InformationView(),
                    const ConnectionView(),
                    appConsole(context),
                    const RegisterView(),
                    const PlatformLogView(),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class ConBottomNavigation extends StatefulWidget {
  final ValueSetter<int> onPressed;
  int setIndex;

  ConBottomNavigation({
    super.key,
    required this.onPressed,
    required this.setIndex,
  });

  @override
  State<ConBottomNavigation> createState() => _ConBottomNavigationState();
}

class _ConBottomNavigationState extends State<ConBottomNavigation> {
  @override
  Widget build(BuildContext context) {
    return Container(
      decoration: const BoxDecoration(
        border: Border(
          top: BorderSide(
            color: AppColor.background,
            width: 2.0,
          ),
        ),
      ),
      child: BottomNavigationBar(
        items: const <BottomNavigationBarItem>[
          BottomNavigationBarItem(
            icon: Icon(CustomIcons.system_update_alt),
            label: 'Обновление',
            //    backgroundColor: Colors.pink,
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.info_outline),
            label: 'Информация',
            //    backgroundColor: Colors.pink,
          ),
          BottomNavigationBarItem(
            icon: Icon(CustomIcons.plug),
            label: 'Подключение',
            //  backgroundColor: Colors.red,
          ),
          BottomNavigationBarItem(
            icon: Icon(CustomIcons.developer_board),
            label: 'Консоль',
            //  backgroundColor: Colors.green,
          ),
          BottomNavigationBarItem(
            icon: Icon(CustomIcons.microchip),
            label: 'Регситры',
            //     backgroundColor: Colors.purple,
          ),
          BottomNavigationBarItem(
            icon: Icon(CustomIcons.format_align_left),
            label: 'Журнал',
            //    backgroundColor: Colors.pink,
          ),
        ],
        currentIndex: widget.setIndex,
        selectedItemColor: AppColor.active,
        onTap: (int index) {
          widget.onPressed(index);
        },
      ),
    );
  }
}

Widget statusTableTextHeader(String text) {
  return Container(
    padding: const EdgeInsets.all(6),
    child: Center(
      child: Text(
        text,
        style: const TextStyle(fontWeight: FontWeight.w500),
      ),
    ),
  );
}

Widget statusTableTextParam(String text, {bool center = true}) {
  if (center) {
    return Container(
      padding: const EdgeInsets.all(6),
      child: Center(
        child: Text(
          text,
        ),
      ),
    );
  } else {
    return Container(
      padding: const EdgeInsets.all(6),
      child: Text(
        text,
      ),
    );
  }
}
