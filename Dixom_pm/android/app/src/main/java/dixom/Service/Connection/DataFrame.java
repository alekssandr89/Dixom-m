package dixom.Service.Connection;

import android.util.Base64;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

import dixom.Const;
import dixom.Crc16;

/**
 * Класс DataFrame представляет структуру данных для одного сообщения,
 * полученного через различные интерфейсы связи (Bluetooth, USB и др.).
 * <p>
 * Он инкапсулирует управляющие байты, тип сообщения, полезные данные,
 * контрольную сумму CRC16, а также метаданные, такие как флаги повторного сообщения
 * и необходимости ответа.
 * </p>
 *
 * <p>Основные возможности класса:</p>
 * <ul>
 *     <li>Декодирование управляющего блока из Base64</li>
 *     <li>Поддержка форматов данных Base64 и UTF-8</li>
 *     <li>Проверка CRC16 для контроля целостности данных</li>
 *     <li>Определение флагов подтверждения и повторного сообщения</li>
 * </ul>
 *
 */
public class DataFrame {

    /** Буфер с управляющей частью сообщения (frameControl) */
    private final ByteBuffer frameControl;

    /** Тип сообщения (нижние 6 бит младшего байта frameControl) */
    private final char msgType;

    /** Флаг: true — нужно отправить подтверждение CRC контроллеру */
    private final boolean response;

    /** Флаг: true — сообщение повторное, false — новое */
    private final boolean repeated;

    /** Контрольная сумма CRC16 для данных */
    private final int crc;

    /** Интерфейс, через который пришли данные (Bluetooth, USB и др.) */
    private final Const.Transprort transportInterface;

    /** Буфер с байтами сообщения */
    private final ByteBuffer buffer;

    /** Буфер с данными в виде строки (UTF-8) */
    private final String stringBuffer;

    /** Количество байт в буфере */
    private final int length;

    /**
     * Конструктор для создания пустого сообщения заданного типа и длины.
     *
     * @param transport интерфейс, через который будет обрабатываться сообщение
     * @param msgType тип сообщения
     * @param length длина буфера для данных
     */
    public DataFrame(Const.Transprort transport, char msgType, int length) {
        this.msgType = msgType;
        this.length = length;
        this.transportInterface = transport;
        this.buffer = ByteBuffer.allocate(length);
        this.frameControl = ByteBuffer.allocate(4);
        this.stringBuffer = "";
        this.response = false;
        this.repeated = true;
        this.crc = -1;
    }

    /**
     * Конструктор для создания DataFrame из строки данных.
     * <p>
     * Строка разбирается на управляющий блок (frameControl) и данные.
     * Данные могут быть в Base64 или UTF-8.
     * Также вычисляется CRC и устанавливаются флаги repeated и response.
     * </p>
     *
     * @param transport интерфейс, через который пришли данные
     * @param data строка с данными сообщения
     */
    public DataFrame(Const.Transprort transport, String data) {

        // Если используется протокол FSC с командой AT+SPPSEND, обрезаем префикс
        if (data.startsWith("AT+SPPSEND")) {
            data = data.substring(data.indexOf(',') + 1);
        }

        // Декодируем первые 4 символа base64 в управляющий блок frameControl
        frameControl = ByteBuffer.wrap(Base64.decode(data.substring(0, 4), Base64.DEFAULT));

        // Остаток строки — данные
        stringBuffer = data.substring(4);

        // Извлекаем тип сообщения (6 младших бит первого байта frameControl)
        msgType = (char) (frameControl.get(0) & 0x3F);

        // Декодируем данные по типу сообщения
        if (msgType == Const.MsgType.BASE64_WHOLE) {
            buffer = ByteBuffer.wrap(Base64.decode(stringBuffer, Base64.DEFAULT));
        } else if (msgType == Const.MsgType.UTF8_WHOLE) {
            byte[] byteArray = stringBuffer.getBytes(StandardCharsets.UTF_8);
            buffer = ByteBuffer.wrap(byteArray, 0, byteArray.length);
        } else {
            // Если тип неизвестен — используем заглушку
            byte[] byteArray = "1 1 1".getBytes(StandardCharsets.UTF_8);
            buffer = ByteBuffer.wrap(byteArray, 0, byteArray.length);
        }

        // Получаем CRC16, считанное из STM32 (из байт 1 и 2 frameControl)
        int receivedCrc = ((frameControl.get(1) & 0xFF) << 8) | (frameControl.get(2) & 0xFF);

        // Проверяем CRC16 по вычислению
        if (receivedCrc == Crc16.arcCrc16(buffer)) {
            crc = receivedCrc;
        } else {
            crc = -1; // Ошибка CRC
        }

        // Определяем флаги повторного сообщения (бит 7) и необходимости ответа (бит 6)
        repeated = ((frameControl.get(0) >> 7) & 0x01) == 1;
        response = ((frameControl.get(0) >> 6) & 0x01) == 1;

        length = buffer.capacity();
        transportInterface = transport;
    }

    /**
     * Возвращает данные сообщения в виде строки.
     * @return данные в виде UTF-8 строки
     */
    public String getCommand() {
        return stringBuffer;
    }

    /**
     * Возвращает массив байт данных сообщения.
     * @return байтовый массив с данными сообщения
     */
    public byte[] getBufferArray() {
        return buffer.array();
    }

    /**
     * Возвращает буфер с байтами данных сообщения.
     * @return ByteBuffer с данными сообщения
     */
    public ByteBuffer getBuffer() {
        return buffer;
    }

    /**
     * Возвращает длину буфера данных.
     * @return длина буфера в байтах
     */
    public int getLength() {
        return length;
    }

    /**
     * Возвращает контрольную сумму CRC16 сообщения.
     * @return значение CRC16, либо -1 при ошибке проверки
     */
    public int getCrc16() {
        return crc;
    }

    /**
     * Проверяет, требуется ли отправка подтверждения CRC.
     * @return true, если требуется ответ (ACK)
     */
    public boolean isResponseRequired() {
        return response;
    }

    /**
     * Проверяет, является ли сообщение повторным.
     * @return true, если сообщение повторное
     */
    public boolean isRepeated() {
        return repeated;
    }

    /**
     * Возвращает интерфейс, через который пришли данные.
     * @return интерфейс передачи данных (Bluetooth, USB и др.)
     */
    public Const.Transprort getTransportInterface() {
        return transportInterface;
    }

    /**
     * Возвращает тип сообщения.
     * @return тип сообщения (6 младших бит первого байта управляющего блока)
     */
    public char getMsgType() {
        return msgType;
    }
}
