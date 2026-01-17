import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/pages/top_bar/register/backend/register.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

import '../../../../driver/device_rx_tx/exchange_param.dart';

class RegisterView extends StatefulWidget {
  const RegisterView({
    super.key,
  });

  @override
  State<RegisterView> createState() => _NumberSystemsViewState();
}

NumberSystemsModel getNew(ControlDevice index) {
  return NumberSystemsModel(
    address: 0,
    register: 0,
    bitRegister: List.generate(32, (i) => (0 & (1 << i)) != 0),
    chipIndex: index,
    maxSimbolAddrHex: 2,
    maxSimbolAddrDec: 3,
    maxSimbolRegHex: 2,
    maxSimbolRegDec: 3,
  );
}

class _NumberSystemsViewState extends State<RegisterView> {
  NumberSystemsModel regValue = getNew(ControlDevice.dacAk4440);
  bool autoRead = false;

  final TextEditingController _controllerRegHex = TextEditingController(); //поле hex регистра
  final TextEditingController _controllerRegDec = TextEditingController(); //поле dec регистра
  final TextEditingController _controllerAddrHex = TextEditingController(); //поле hex адреса
  final TextEditingController _controllerAddrDec = TextEditingController(); //поле dec адреса

  @override
  void initState() {
    super.initState();
    registerControlDefault(ControlDevice.dacAk4440);
  }

  void getInfo() async {
    final List<NumberSystemsModel> newValuer = await deviceResponse<NumberSystemsModel>(
      standard: getNew(regValue.chipIndex),
      request: registerControlCmd(chipNum: regValue.chipIndex.index, regAddr: regValue.address),
      function: registerControlFilter,
    );
    enterRegVal(newValuer[0].register);
  }

  void sendInfo() {}

  void setBitState(bool value, int num) {
    regValue.bitRegister[num] = value;

    regValue.register = convertBoolToInteger(regValue.bitRegister);
    _controllerRegDec.text = regValue.register.toString();
    _controllerRegHex.text = regValue.register.toRadixString(16).toUpperCase();
    setState(() {});
  }

  void upAddrBtn() {
    regValue.address++;
    enterAddrVal(regValue.address);
  }

  void downAddrBtn() {
    regValue.address--;
    enterAddrVal(regValue.address);
  }

  void upRegBtn() {
    regValue.register++;
    enterRegVal(regValue.register);
  }

  void downRegBtn() {
    regValue.register--;
    enterRegVal(regValue.register);
  }

  void enterAddrVal(int val) {
    if (val > getLimit(regValue.chipIndex.addrBitDepth)) {
      val = 0;
    }

    if (val < 0) {
      val = getLimit(regValue.chipIndex.addrBitDepth);
    }

    regValue.address = val;

    String strValDec = regValue.address.toString();
    _controllerAddrDec.value = TextEditingValue(
      text: strValDec,
      selection: TextSelection.collapsed(offset: strValDec.length),
    );

    String strValHex = regValue.address.toRadixString(16).toUpperCase();
    _controllerAddrHex.value = TextEditingValue(
      text: strValHex,
      selection: TextSelection.collapsed(offset: strValHex.length),
    );
    if (autoRead) {
      getInfo();
    } else {
      setState(() {});
    }
  }

  void enterRegVal(int val) {
    if (val > getLimit(regValue.chipIndex.regBitDepth)) {
      val = 0;
    }

    if (val < 0) {
      val = getLimit(regValue.chipIndex.regBitDepth);
    }

    regValue.register = val;
    regValue.bitRegister = List.generate(32, (i) => (regValue.register & (1 << i)) != 0);

    String strValDec = regValue.register.toString();
    _controllerRegDec.value = TextEditingValue(
      text: strValDec,
      selection: TextSelection.collapsed(offset: strValDec.length),
    );

    String strValHex = regValue.register.toRadixString(16).toUpperCase();
    _controllerRegHex.value = TextEditingValue(
      text: strValHex,
      selection: TextSelection.collapsed(offset: strValHex.length),
    );
    setState(() {});
  }

  int getLimit(int bitDepth) {
    int val = 0;
    if (bitDepth == 8) {
      val = 255;
    } else if (bitDepth == 16) {
      val = 65535;
    } else if (bitDepth == 32) {
      val = 4294967295;
    }
    return val;
  }

  List<String> chipNameList = ControlDevice.values.map((device) => device.decoding).toList();

  void selectChip(ControlDevice index) {
    regValue.chipIndex = index;

    if (regValue.chipIndex.addrBitDepth == 8) {
      regValue.maxSimbolAddrDec = 3;
      regValue.maxSimbolAddrHex = 2;
    } else if (regValue.chipIndex.addrBitDepth == 16) {
      regValue.maxSimbolAddrDec = 5;
      regValue.maxSimbolAddrHex = 4;
    } else if (regValue.chipIndex.addrBitDepth == 32) {
      regValue.maxSimbolAddrDec = 10;
      regValue.maxSimbolAddrHex = 8;
    }

    if (regValue.chipIndex.regBitDepth == 8) {
      regValue.maxSimbolRegDec = 3;
      regValue.maxSimbolRegHex = 2;
    } else if (regValue.chipIndex.regBitDepth == 16) {
      regValue.maxSimbolRegDec = 5;
      regValue.maxSimbolRegHex = 4;
    } else if (regValue.chipIndex.regBitDepth == 32) {
      regValue.maxSimbolRegDec = 10;
      regValue.maxSimbolRegHex = 8;
    }
    enterAddrVal(0);
    enterRegVal(0);
  }

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Container(
        padding: const EdgeInsets.all(AppIndents.pading),
        decoration: const BoxDecoration(
          color: AppColor.secondary,
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                DropdownButton<ControlDevice>(
                  value: regValue.chipIndex,
                  icon: const Icon(Icons.arrow_downward),
                  elevation: 16,
                  onChanged: (ControlDevice? chip) {
                    registerControlDefault(chip!);
                  },
                  items: ControlDevice.values.map((ControlDevice chip) {
                    return DropdownMenuItem<ControlDevice>(
                      value: chip,
                      child: Text(chipNameList[chip.index]),
                    );
                  }).toList(),
                ),
                const SizedBox(width: 16),
                Row(
                  children: [
                    Checkbox(
                      value: autoRead,
                      onChanged: (bool? newState) {
                        autoRead = newState!;
                        setState(() {});
                      },
                    ),
                    const Text("Авточтение")
                  ],
                ),
                if (!Responsive.isMobile(context)) const SizedBox(width: 16),
                if (!Responsive.isMobile(context)) controlButton(context),
              ],
            ),
            if (Responsive.isMobile(context)) const SizedBox(height: 20),
            if (Responsive.isMobile(context)) controlButton(context),
            // if (Responsive.isTablet(context)) controlButton(),
            const SizedBox(height: 20),
            enterAddress(),
            const SizedBox(height: 20),
            enterRegister(),
            const SizedBox(height: 30),
            if (Responsive.isDesktop(context)) bitViewDesktop(),
            if (Responsive.isTablet(context)) bitViewTablet(context),
            if (Responsive.isMobile(context)) bitViewMobile()
          ],
        ),
      ),
    );
  }

  Row bitViewDesktop() {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        if (regValue.chipIndex.regBitDepth > 16) bitContainer(0),
        const SizedBox(height: 5),
        if (regValue.chipIndex.regBitDepth > 16) bitContainer(8),
        const SizedBox(height: 5),
        if (regValue.chipIndex.regBitDepth > 8) bitContainer(16),
        const SizedBox(height: 5),
        bitContainer(24),
      ],
    );
  }

  Column bitViewTablet(BuildContext context) {
    return Column(
      children: [
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            if (regValue.chipIndex.regBitDepth > 16) bitContainer(0),
            const SizedBox(height: 5),
            if (regValue.chipIndex.regBitDepth > 16) bitContainer(8),
            const SizedBox(height: 5),
          ],
        ),
        if (Responsive.isTablet(context))
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              if (regValue.chipIndex.regBitDepth > 8) bitContainer(16),
              const SizedBox(height: 5),
              bitContainer(24),
            ],
          ),
      ],
    );
  }

  Column bitViewMobile() {
    return Column(
      children: [
        if (regValue.chipIndex.regBitDepth > 16) bitContainer(0),
        const SizedBox(height: 5),
        if (regValue.chipIndex.regBitDepth > 16) bitContainer(8),
        const SizedBox(height: 5),
        if (regValue.chipIndex.regBitDepth > 8) bitContainer(16),
        const SizedBox(height: 5),
        bitContainer(24),
      ],
    );
  }

  Row controlButton(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceAround,
      children: [
        ElevatedButton.icon(
          onPressed: () {
            getInfo();
          },
          icon: const Icon(Icons.get_app_sharp), // Иконка
          label: const Text('Прочитать'), // Текст кнопки
        ),
        if (!Responsive.isMobile(context)) const SizedBox(width: 16),
        ElevatedButton.icon(
          onPressed: () {
            sendInfo();
          },
          icon: const Icon(Icons.upload), // Иконка
          label: const Text('Записать'), // Текст кнопки
        ),
      ],
    );
  }

  Row enterAddress() {
    return Row(
      children: [
        ElevatedButton(onPressed: downAddrBtn, child: const Text("-")),
        const SizedBox(width: 10),
        Expanded(
          child: TextField(
            textAlign: TextAlign.center,
            controller: _controllerAddrDec,
            maxLength: regValue.maxSimbolAddrDec,
            decoration: const InputDecoration(labelText: 'Address DEC', contentPadding: EdgeInsets.only(top: 5.0)),
            inputFormatters: [
              FilteringTextInputFormatter.digitsOnly, // Ограничиваем ввод только цифрами
            ],
            onChanged: (text) {
              enterAddrVal(int.tryParse(text) ?? 0);
            },
          ),
        ),
        const SizedBox(width: 10),
        Expanded(
          child: TextField(
            textAlign: TextAlign.center,
            controller: _controllerAddrHex,
            maxLength: regValue.maxSimbolAddrHex,
            decoration: const InputDecoration(
              labelText: 'Address HEX',
              contentPadding: EdgeInsets.only(top: 5.0),
            ),
            inputFormatters: [
              FilteringTextInputFormatter.allow(RegExp('[0-9a-fA-F]+')), // Ограничиваем ввод только HEX
            ],
            onChanged: (text) {
              if (text == "") {
                text = "0";
              }
              enterAddrVal(int.parse(text, radix: 16));
            },
          ),
        ),
        const SizedBox(width: 10),
        ElevatedButton(onPressed: upAddrBtn, child: const Text("+")),
      ],
    );
  }

  Row enterRegister() {
    return Row(
      children: [
        ElevatedButton(onPressed: downRegBtn, child: const Text("-")),
        const SizedBox(width: 10),
        Expanded(
          child: TextField(
            textAlign: TextAlign.center,
            controller: _controllerRegDec,
            maxLength: regValue.maxSimbolRegDec,
            decoration: const InputDecoration(labelText: 'Register DEC', contentPadding: EdgeInsets.only(top: 5.0)),
            inputFormatters: [
              FilteringTextInputFormatter.digitsOnly, // Ограничиваем ввод только цифрами
            ],
            onChanged: (text) {
              enterRegVal(int.tryParse(text) ?? 0);
            },
          ),
        ),
        const SizedBox(width: 10),
        Expanded(
          child: TextField(
            textAlign: TextAlign.center,
            controller: _controllerRegHex,
            maxLength: regValue.maxSimbolRegHex,
            inputFormatters: [
              FilteringTextInputFormatter.allow(RegExp('[0-9a-fA-F]+')), // Ограничиваем ввод только HEX
            ],
            decoration: const InputDecoration(labelText: 'Register HEX', contentPadding: EdgeInsets.only(top: 5.0)),
            onChanged: (text) {
              if (text == "") {
                text = "0";
              }
              enterRegVal(int.parse(text, radix: 16));
            },
          ),
        ),
        const SizedBox(width: 10),
        ElevatedButton(onPressed: upRegBtn, child: const Text("+")),
      ],
    );
  }

  SizedBox bitContainer(int maxBit) {
    return SizedBox(
      height: 65,
      child: Center(
        child: Wrap(
          spacing: -4.0,
          children: [
            for (int i = maxBit; i < 8 + maxBit; i++) bitWiget(31 - i, regValue.bitRegister[31 - i], setBitState),
          ],
        ),
      ),
    );
  }

  Column bitWiget(int num, bool state, Function(bool, int)? onChanged) {
    return Column(
      children: [
        Checkbox(
          value: state,
          onChanged: (bool? newState) {
            onChanged?.call(newState!, num);
          },
        ),
        Text(num.toString()),
      ],
    );
  }

  NumberSystemsModel registerControlDefault(ControlDevice index) {
    regValue.address = 0;
    regValue.register = 0;
    for (int i = 0; i < regValue.bitRegister.length; i++) {
      regValue.bitRegister[i] = false;
    }
    selectChip(index);
    return regValue;
  }
}

String registerControlCmd({required int chipNum, required int regAddr}) {
  return "${RootPacked.general.index} ${General.register.index} $chipNum $regAddr";
}

int convertBoolToInteger(List<bool> valBin) {
  int result = 0;
  for (int i = 0; i < 32; i++) {
    if (valBin[31 - i]) {
      result |= (1 << (31 - i));
    }
  }
  return result;
}

WorkState registerControlFilter(CommandMsg msg, List<NumberSystemsModel> response) {
  if (msg.packed == RootPacked.general.index && msg.parametr == General.register.index) {
    if (msg.podParmetr == Register.register.index) {
      int addres = int.parse(msg.split[SetRegister.addres.index]);
      int value = int.parse(msg.split[SetRegister.value.index]);

      response[0] = NumberSystemsModel(
        address: addres,
        register: value,
        bitRegister: List.generate(32, (i) => (value & (1 << i)) != 0),
        chipIndex: ControlDevice.dacAk4440,
        maxSimbolAddrHex: 2,
        maxSimbolAddrDec: 3,
        maxSimbolRegHex: 2,
        maxSimbolRegDec: 3,
      );
      return WorkState.complete;
    }
  }
  return WorkState.notFound;
}

class NumberSystemsModel {
  List<bool> bitRegister; // регистр разбитый на биты
  int register; //значение регистра
  int address; //адрес регистра
  ControlDevice chipIndex; //Индекс выбранной микросхемы
  int maxSimbolAddrHex; //количество сиволов в hex поле адреса
  int maxSimbolAddrDec; //количество сиволов в десятичном поле адреса
  int maxSimbolRegHex; //количество сиволов в hex поле регистра
  int maxSimbolRegDec; //количество сиволов в десятичном поле регистра

  NumberSystemsModel({
    required this.bitRegister,
    required this.register,
    required this.address,
    required this.chipIndex,
    required this.maxSimbolAddrHex,
    required this.maxSimbolAddrDec,
    required this.maxSimbolRegHex,
    required this.maxSimbolRegDec,
  });
}

enum ControlDevice {
  dacAk4440("I2C2 Цап Ak4440", 8, 8), //0
  dacAdau1962("I2C2 Цап Adau1962", 8, 8), //1
  dacAk4458vn("I2C2 Цап Ak4458", 8, 8), //2
  fmRadioRda5807("I2C1 Фм радио Rda5807", 16, 16), //3
  rtcDs3231("I2C2 часы RTC DS3231", 8, 8), //4
  amplifierTAS321("I2C2 Усилитель TAS3251", 8, 8), //5
  fmRadioSi4735("I2C1 Фм радио SI4735", 16, 16), //6
  dspAdau1452("SPI1 DSP ADAU1452", 16, 32), //7
  eeprom24lc256("I2C2 EEPROM 24LC256", 16, 8), //8
  flash25q80dv("SPI1 FLASH 25Q80DV", 16, 8), //9
  amplifierTdf8551J("I2C1 Усилитель TDF8551J", 8, 8), //10
  dacPcm1690("I2C2 ЦАП PCM1690", 8, 8), //11
  stm32f405("STM32F405RGT6", 32, 32); //12

  final String decoding; //Описание
  final int addrBitDepth; //максимальное количество адресов
  final int regBitDepth; //максималный размер регистра

  const ControlDevice(this.decoding, this.addrBitDepth, this.regBitDepth);
}
