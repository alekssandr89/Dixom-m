import 'dart:async';
import 'dart:io';
import 'dart:typed_data';
import 'package:dixom/main.dart';
import 'update_const.dart';
import 'update_model.dart';

Future<dynamic> downloadFirmware({required String url, required int fileSize}) async {
  final client = HttpClient();
  final bytes = Uint8List(fileSize);
  int pointer = 0;
  final urlF = Uri.parse(url);
  try {
    final request = await client.getUrl(urlF);
    final response = await request.close();
    int totalLoadSize = 0;
    await for (var event in response) {
      for (int i = 0; i < event.length; i++) {
        bytes[pointer] = event[i];
        pointer++;
        totalLoadSize++;

        appRef.read(frimwareUpdateViewPrdr.notifier).update(progress: (totalLoadSize / fileSize * 100) / 100);
      }
      if (appRef.read(frimwareUpdateViewPrdr.select((value) => value.btnAction == UpdateBtnAction.download))) {
        appRef.read(frimwareUpdateViewPrdr.notifier).update(status: UpdateStatus.cancel, progress: 0);
        break;
      }
    }
    if (appRef.read(frimwareUpdateViewPrdr.select((value) => value.btnAction != UpdateBtnAction.download))) {
      appRef.read(frimwareUpdateViewPrdr.notifier).update(progress: 1);
    }

    return bytes;
  } catch (error) {
    return UpdateStatus.appDownloadError;
  }
}
