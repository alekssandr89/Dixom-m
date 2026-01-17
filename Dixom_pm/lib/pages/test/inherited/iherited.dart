import 'package:flutter/material.dart';

class ExampleInherited extends StatelessWidget {
  const ExampleInherited({super.key});

  @override
  Widget build(BuildContext context) {
    return const Scaffold(
      body: SafeArea(
        child: DataOwnerStatefull(),
      ),
    );
  }
}

class DataOwnerStatefull extends StatefulWidget {
  const DataOwnerStatefull({super.key});

  @override
  DataOwnerStatefullState createState() => DataOwnerStatefullState();
}

class DataOwnerStatefullState extends State<DataOwnerStatefull> {
  var _valueOne = 0;
  var _valueTwo = 0;

  void _incrimentOne() {
    _valueOne += 1;
    setState(() {});
  }

  void _incrimentTwo() {
    _valueTwo += 1;
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.center,
      children: [
        ElevatedButton(
          onPressed: _incrimentOne,
          child: const Text('Жми раз'),
        ),
        ElevatedButton(
          onPressed: _incrimentTwo,
          child: const Text('Жми два'),
        ),
        DataProviderInherited(
          valueOne: _valueOne,
          valueTwo: _valueTwo,
          child: const DataConsumerStateless(),
        ),
      ],
    );
  }
}

class DataConsumerStateless extends StatelessWidget {
  const DataConsumerStateless({super.key});

  @override
  Widget build(BuildContext context) {
    final value = context.dependOnInheritedWidgetOfExactType<DataProviderInherited>(aspect: 'one')?.valueOne ?? 0;
    return Column(
      crossAxisAlignment: CrossAxisAlignment.center,
      children: [
        Text('$value'),
        const DataConsumerStatefull(),
      ],
    );
  }
}

class DataConsumerStatefull extends StatefulWidget {
  const DataConsumerStatefull({super.key});

  @override
  DataConsumerStatefullState createState() => DataConsumerStatefullState();
}

class DataConsumerStatefullState extends State<DataConsumerStatefull> {
  @override
  Widget build(BuildContext context) {
    /* getInherit<DataProviderInherited>(context)?.valueTwo;
    final value = getInherit<DataProviderInherited>(context)?.valueTwo;*/
    final value2 = context.dependOnInheritedWidgetOfExactType<DataProviderInherited>(aspect: 'two')?.valueTwo ?? 0;
    return Text('$value2');
  }
}

class DataProviderInherited extends InheritedModel<String> {
  final int valueOne;
  final int valueTwo;
  const DataProviderInherited({
    super.key,
    required this.valueOne,
    required this.valueTwo,
    required super.child,
  });

  @override
  bool updateShouldNotify(DataProviderInherited oldWidget) {
    return valueOne != oldWidget.valueOne || valueTwo != oldWidget.valueTwo;
  }

  @override
  bool updateShouldNotifyDependent(
    covariant DataProviderInherited oldWidget,
    Set<String> dependencies,
  ) {
    final isValueOneUpdated = valueOne != oldWidget.valueOne && dependencies.contains('one');
    final isValueTwoUpdated = valueOne != oldWidget.valueTwo && dependencies.contains('two');
    return isValueOneUpdated || isValueTwoUpdated;
  }
}

T? getInherit<T>(BuildContext context) {
  final element = context.getElementForInheritedWidgetOfExactType<DataProviderInherited>();
  final widget = element?.widget;
  if (widget is T) {
    return widget as T;
  } else {
    return null;
  }
}
