/**
 * Пакет dixom.Service.Connection содержит интерфейсы и классы,
 * отвечающие за управление и обработку событий транспортных соединений
 * в рамках сервиса Dixom.
 *
 * В частности, здесь определён интерфейс {@link TransportEventListener},
 * который используется для получения уведомлений о статусе подключения
 * и приходящих данных с различных транспортных средств (например, USB, Bluetooth).
 */

package dixom.Service.Connection;

import dixom.Const;

/**
 * Интерфейс для прослушивания событий транспортного слоя.
 * <p>
 * Реализации данного интерфейса получают уведомления о состоянии соединения
 * и о новых поступивших данных через различные методы транспортировки.
 * Это позволяет централизованно обрабатывать изменения состояния и полученные данные
 * вне зависимости от конкретной технологии соединения.
 * </p>
 *
 * Методы:
 * <ul>
 *   <li>{@link #onTransportStatus(String, Const.Transprort, Const.WorkState, int, int, int, int)} — уведомление об изменении статуса транспорта,
 *       включая количество успешных и ошибочных операций передачи и приёма;</li>
 *   <li>{@link #onDataReceived(DataFrame)} — уведомление о получении нового блока данных {@link DataFrame}.</li>
 * </ul>
 *
 * @see dixom.Const.Transprort
 * @see dixom.Const.WorkState
 * @see DataFrame
 */
public interface TransportEventListener {

    /**
     * Вызывается при изменении статуса транспортного соединения.
     *
     * @param method      строка с идентификатором метода для передачи во Flutter,
     *                    позволяющая корректно обработать событие на стороне Flutter.
     * @param transport   тип транспорта из перечисления {@link Const.Transprort}
     * @param status      текущее состояние соединения из {@link Const.WorkState}
     * @param successIn   количество успешно принятых пакетов
     * @param successOut  количество успешно отправленных пакетов
     * @param errorIn     количество ошибок при приёме
     * @param errorOut    количество ошибок при отправке
     */
    void onTransportStatus(
            String method,
            Const.Transprort transport,
            Const.WorkState status,
            int successIn,
            int successOut,
            int errorIn,
            int errorOut
    );

    /**
     * Вызывается при получении новых данных через транспортный слой.
     *
     * @param dataFrame объект {@link DataFrame}, содержащий данные, полученные по соединению
     */
    void onDataReceived(DataFrame dataFrame);
}
