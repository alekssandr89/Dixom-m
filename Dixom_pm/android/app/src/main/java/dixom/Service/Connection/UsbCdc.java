package dixom.Service.Connection;

import android.content.Context;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;

import java.util.List;

import dixom.Config;
import dixom.Service.Connection.UsbCdcDriver.CdcAcmSerialDriver;
import dixom.Service.Connection.UsbCdcDriver.ProbeTable;
import dixom.Service.Connection.UsbCdcDriver.UsbSerialDriver;
import dixom.Service.Connection.UsbCdcDriver.UsbSerialPort;
import dixom.Service.Connection.UsbCdcDriver.UsbSerialProber;
import dixom.Service.Connection.UsbCdcDriver.Util.SerialInputOutputManager;
import dixom.Const;

public class UsbCdc implements SerialInputOutputManager.Listener{


    private UsbSerialPort UsbPortCDC;
    private UsbDeviceConnection connection;
    private final UsbManager usbManager;
    private SerialInputOutputManager UsbCdcIOManager;
    private final ConnectionManager connectionManager;
    public final TransportStatus transportStatus;
    public UsbCdc(Context context, ConnectionManager connectionManager){
        transportStatus = new TransportStatus(Const.Transprort.USB_CDC, Const.WorkState.NOT_INITIALIZED, 0, 0, 0, 0);                  //Состояние интерфейса Usb CDC

        this.connectionManager = connectionManager;
        usbManager = (UsbManager) context.getSystemService(Context.USB_SERVICE);

    }

    public Const.WorkState Connect(){

        if (transportStatus.getState() != Const.WorkState.CONNECTED){
            transportStatus.setState(Const.WorkState.DISCONNECTED);
            return Const.WorkState.DISCONNECTED;
            //TODO Надо доработать сейчас сразу возвращеается с ошибкой СПЕЦИАЛЬНО!!!
        }



        if(transportStatus.getState() == Const.WorkState.CONNECTED){
            return transportStatus.getState() ;

        }
        transportStatus.setState(Const.WorkState.ERROR);

        ProbeTable customTable = new ProbeTable();
        customTable.addProduct(Const.VENDOR_ID, Const.PRODUCT_ID, CdcAcmSerialDriver.class);
        final UsbSerialProber property = new UsbSerialProber(customTable);


        try {
            //Ищем доступные драйверы
            List<UsbSerialDriver> availableDrivers = property.findAllDrivers(usbManager);

            if (availableDrivers.size()>0){
                // Открываем соединение с первым доступным драйвером.
                UsbSerialDriver driver = availableDrivers.get(0);
                connection = usbManager.openDevice(driver.getDevice());
                if (connection != null) {

                    // Выбираем первый доступный порт
                    UsbPortCDC = driver.getPorts().get(0);

                    if(UsbPortCDC != null){

                        try {
                            transportStatus.setState(Const.WorkState.ERROR);
                            UsbPortCDC.open(connection);
                            UsbPortCDC.setParameters(115200, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);
                            UsbCdcIOManager = new SerialInputOutputManager(UsbPortCDC, this);
                            UsbCdcIOManager.start();
                            transportStatus.setState(Const.WorkState.CONNECTED);

                        } catch (Exception e) {
                            transportStatus.setState(Const.WorkState.ERROR);
                            connectionManager.Disconnect_Interface( Const.Transprort.USB_CDC,  Const.WorkState.ENABLE);
                        }
                    }else{
                        transportStatus.setState(Const.WorkState.ERROR);
                    }
                }else{
                    transportStatus.setState(Const.WorkState.ERROR);
                }
            }else{
                transportStatus.setState(Const.WorkState.NOT_AVAILABLE);
            }
        } catch (Exception e) {
            // обработка исключения
            transportStatus.setState(Const.WorkState.NOT_FOUND);
            e.printStackTrace();
        }
        return  transportStatus.getState();
    }

    public Const.WorkState Disconnect(){

        if(transportStatus.getState() == Const.WorkState.DISCONNECTED){
            return transportStatus.getState() ;
        }

        try {
            UsbCdcIOManager.stop();
            connection.close();
            UsbPortCDC.close();
            transportStatus.setState(Const.WorkState.DISCONNECTED);
            return  transportStatus.getState();
        }catch (Exception e) {
            transportStatus.setState(Const.WorkState.ERROR);
            return  transportStatus.getState();
        }
    }

    public Const.WorkState  Sending_Data(String dataS){
        return Sending_Data(dataS.getBytes());
    }

    public Const.WorkState  Sending_Data(byte[] dataB) {

        Const.WorkState  WorkResult = Const.WorkState.ERROR;

        if(transportStatus.getState() == Const.WorkState.CONNECTED){
            try {

                if(dataB.length < Const.MAX_MESSAGE_LENGTH){
                    byte[] SendData = new byte[dataB.length+3];
                    SendData[0] = (byte)Const.MsgType.UTF8_WHOLE;
                    SendData[1] = (byte)dataB.length;
                    SendData[dataB.length] = (byte)0;

                    for(int i = 0; i < dataB.length; i++){
                        SendData[i+2] = dataB[i];
                    }
                    UsbPortCDC.write(SendData, 600);
                    transportStatus.update(Const.TransferStatus.SUCCESS_OUT);
                    return Const.WorkState.OK;
                }else{
                    return Const.WorkState.ERROR;
                }
            } catch (Exception e) {
            }
        }
        return WorkResult;
    }


    @Override
    public void onNewData(byte[] RxByte) {

        DataFrame NewMessage = new DataFrame(Const.Transprort.USB_CDC, (char)RxByte[1], RxByte[0]);
        NewMessage.getBuffer().put(RxByte, 2, NewMessage.getLength());
        connectionManager.Receiver.addMessageToQueue(NewMessage);
    }

    @Override
    public void onRunError(Exception e) {

    }

}
