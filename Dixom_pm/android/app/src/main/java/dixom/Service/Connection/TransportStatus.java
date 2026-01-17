package dixom.Service.Connection;

import dixom.Const;

/**
 * Класс {@code TransportStatus} представляет текущее состояние и статистику
 * работы транспортного соединения в сервисе Dixom.
 *
 * <p>Объект данного класса хранит тип транспорта, статус соединения, а также
 * статистику успешных и ошибочных операций передачи и приёма данных.
 * Помимо хранения данных, класс реализует механизм уведомления слушателя
 * {@link TransportEventListener} об изменениях статуса или статистики.</p>
 *
 * <p>Используется для централизованного контроля и мониторинга состояния
 * различных видов транспортных интерфейсов, таких как USB, Bluetooth и др.</p>
 *
 * <h3>Основные возможности:</h3>
 * <ul>
 *     <li>Хранение текущего состояния работы транспорта {@link Const.WorkState}</li>
 *     <li>Подсчёт количества успешных и ошибочных входящих и исходящих передач</li>
 *     <li>Уведомление зарегистрированного слушателя {@link TransportEventListener} при изменениях</li>
 * </ul>
 *
 * @see dixom.Const.Transprort
 * @see dixom.Const.WorkState
 * @see dixom.Service.Connection.TransportEventListener;
 */
public class TransportStatus {

    private final Const.Transprort transport;
    private Const.WorkState status;
    private int successIn;
    private int successOut;
    private int errorIn;
    private int errorOut;
    private TransportEventListener transportEventListener;

    /**
     * Создаёт объект состояния транспорта с указанными начальными параметрами.
     *
     * @param transport   тип транспортного интерфейса
     * @param status      начальное состояние работы транспорта
     * @param successIn   количество успешно принятых сообщений
     * @param successOut  количество успешно отправленных сообщений
     * @param errorIn     количество ошибок при приёме
     * @param errorOut    количество ошибок при отправке
     */
    public TransportStatus(Const.Transprort transport, Const.WorkState status,
                           int successIn, int successOut, int errorIn, int errorOut) {
        this.transport = transport;
        this.status = status;
        this.successIn = successIn;
        this.successOut = successOut;
        this.errorIn = errorIn;
        this.errorOut = errorOut;
    }

    /**
     * Устанавливает слушателя для получения уведомлений об изменениях статуса транспорта.
     *
     * @param listener слушатель событий {@link TransportEventListener}
     */
    public void setListener(TransportEventListener listener) {
        this.transportEventListener = listener;
    }

    /**
     * Возвращает текущий статус работы транспорта.
     *
     * @return текущее состояние {@link Const.WorkState}
     */
    public Const.WorkState getState() {
        return status;
    }

    /**
     * Возвращает количество операций передачи или приёма, соответствующих указанному типу.
     *
     * @param state тип статистики передачи {@link Const.TransferStatus}
     * @return количество успешных или ошибочных операций, либо 0 если тип не распознан
     */
    public int getExchangeInfo(Const.TransferStatus state) {
        switch (state) {
            case SUCCESS_IN:
                return successIn;
            case SUCCESS_OUT:
                return successOut;
            case ERROR_IN:
                return errorIn;
            case ERROR_OUT:
                return errorOut;
            default:
                return 0;
        }
    }

    /**
     * Устанавливает новое состояние работы транспорта и уведомляет слушателя.
     *
     * @param state новое состояние {@link Const.WorkState}
     */
    public void setState(Const.WorkState state) {
        this.status = state;
        notifyChanged(Const.METHOD_GET_CONNECTION);
    }

    /**
     * Обновляет статистику передачи/приёма в зависимости от типа события и уведомляет слушателя.
     *
     * @param state тип события передачи {@link Const.TransferStatus}
     */
    public void update(Const.TransferStatus state) {
        switch (state) {
            case SUCCESS_IN:
                successIn++;
                break;
            case SUCCESS_OUT:
                successOut++;
                break;
            case ERROR_IN:
                errorIn++;
                break;
            case ERROR_OUT:
                errorOut++;
                break;
        }
        notifyChanged(Const.METHOD_INTERFACE_INFO);
    }

    /**
     * Вспомогательный метод для уведомления слушателя об изменениях.
     *
     * @param method строковый идентификатор метода/события, который будет отправлен
     *               на Flutter (через механизм вызова методов платформы),
     *               используемый для обработки событий на стороне Flutter.
     */
    private void notifyChanged(String method) {
        if (transportEventListener != null) {
            transportEventListener.onTransportStatus(
                    method,
                    transport,
                    status,
                    successIn,
                    successOut,
                    errorIn,
                    errorOut
            );
        }
    }
}
