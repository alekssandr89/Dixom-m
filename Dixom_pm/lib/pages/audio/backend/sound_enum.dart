enum ChannelId {
  channel_1("Channel 1"),
  channel_2("Channel 2"),
  channel_3("Channel 3"),
  channel_4("Channel 4"),
  channel_5("Channel 5"),
  channel_6("Channel 6"),
  channel_7("Channel 7"),
  channel_8("Channel 8"),
  channel_9("Channel 9"),
  channel_10("Channel 10"),
  channel_11("Channel 11"),
  channel_12("Channel 12"),
  spdifL("SPDIF L"),
  spdifR("SPDIF R");

  final String decoding;
  const ChannelId(this.decoding);
}

enum GroupId {
  group_1("Tweater"),
  group_2("Front"),
  group_3("Rear"),
  group_4("Sabwoower"),
  group_5("Channel 5"),
  group_6("Channel 6"),
  spdif("SPDIF");

  final String decoding;
  const GroupId(this.decoding);
}

enum Volume {
  master, //0
  bluetoothMin, //1
  auxMin, //2
  radioMin, //3
  spdifMin, //4
  usbMin, //5
  bluetoothMax, //6
  auxMax, //7
  radioMax, //8
  spdifMax, //9
  masterDsp, //10
  usb, //11
  usbL, //12
  usbR, //13
  bluetooth, //14
  bluetoothL, //15
  bluetoothR, //16
  aux, //17
  auxL, //18
  auxR, //19
  radio, //20
  radioL, //21
  radioR, //22
  spdif, //23
  spdifL, //24
  spdifR, //25
  mic, //26
  micL, //27
  micR, //28
  ch1L_2R, //29
  ch1L, //30
  ch2R, //31
  ch3L_4R, //32
  ch3L, //33
  ch4R, //34
  ch5L_6R, //35
  ch5L, //36
  ch6R, //37
  ch7L_8R, //38
  ch7L, //39
  ch8R, //34
  ch9L_10R, //35
  ch9L, //36
  ch10R, //37
  ch11L_12R, //38
  ch11L, //39
  ch12R, //40
  spdifOutLR, //41
  spdifOutL, //42
  spdifOutR, //43
}

enum BandControl {
  band1, //1
  band2, //2
  band3, //3
  band4, //4
  band5, //5
  band6, //6
  band7, //7
  band8, //8
  band9, //9
  band10, //10
  band11, //11
  band12, //12
  band13, //13
  band14, //14
  hiPass, //Hi Pass
  lowPass //Low Pass
}

enum EqSourceChannel {
  usb, //0
  aux, //1
  bluetooth, //2
  fmradio, //3
  spdif, //4
  microphone, //5
}
