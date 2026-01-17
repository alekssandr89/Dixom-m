import 'package:flutter/material.dart';

class Example extends StatelessWidget {
  const Example({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: SimpleCalcWidget(),
      ),
    );
  }
}

class SimpleCalcWidget extends StatelessWidget {
  SimpleCalcWidget({super.key});

  final _model = SimpleCalcWidgetModel();

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 30),
        child: ChangeNotifierProvider(
          model: _model,
          child: const Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              FirstNumberWidget(),
              SizedBox(height: 10),
              SecondNumberWidget(),
              SizedBox(height: 10),
              SummButtonWidget(),
              SizedBox(height: 10),
              ResultWidget(),
            ],
          ),
        ),
      ),
    );
  }
}

class FirstNumberWidget extends StatelessWidget {
  const FirstNumberWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return TextField(
      decoration: const InputDecoration(border: OutlineInputBorder()),
      onChanged: (String value) => ChangeNotifierProvider.read<SimpleCalcWidgetModel>(context)?.firstNumber = value,
    );
  }
}

class SecondNumberWidget extends StatelessWidget {
  const SecondNumberWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return TextField(
      decoration: const InputDecoration(border: OutlineInputBorder()),
      onChanged: (String value) => ChangeNotifierProvider.read<SimpleCalcWidgetModel>(context)?.secondNumber = value,
    );
  }
}

class SummButtonWidget extends StatelessWidget {
  const SummButtonWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return ElevatedButton(
      onPressed: () => ChangeNotifierProvider.read<SimpleCalcWidgetModel>(context)?.summ(),
      child: const Text('Посчитать сумму'),
    );
  }
}

class ResultWidget extends StatelessWidget {
  const ResultWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final value = ChangeNotifierProvider.watch<SimpleCalcWidgetModel>(context)?.summResult ?? '-1';
    return Text('Результат: $value');
  }
}

class SimpleCalcWidgetModel extends ChangeNotifier {
  int? _firstNumber;
  int? _secondNumber;
  int? summResult;

  set firstNumber(String value) => _firstNumber = int.tryParse(value);
  set secondNumber(String value) => _secondNumber = int.tryParse(value);

  void summ() {
    if (_firstNumber != null && _secondNumber != null) {
      summResult = _firstNumber! + _secondNumber!;
    } else {
      summResult = -1;
    }
    notifyListeners();
  }
}

class ChangeNotifierProvider<T extends ChangeNotifier> extends InheritedNotifier<T> {
  const ChangeNotifierProvider({
    super.key,
    required T model,
    required super.child,
  }) : super(
          notifier: model,
        );

  static T? watch<T extends ChangeNotifier>(BuildContext context) {
    return context.dependOnInheritedWidgetOfExactType<ChangeNotifierProvider<T>>()?.notifier;
  }

  static T? read<T extends ChangeNotifier>(BuildContext context) {
    final widget = context.getElementForInheritedWidgetOfExactType<ChangeNotifierProvider<T>>()?.widget;
    if (widget is ChangeNotifierProvider<T>) {
      return widget.notifier;
    } else {
      return null;
    }
  }
}
