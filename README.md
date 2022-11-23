# Rfid_rc522_i2c_linux
Driver for module rc522 using i2c

1.Setup: 

    sudo nano /etc/systemd/system/dbus-org.bluez.service 
   
Sửa dòng ExecStart thành: 

    ExecStart=/usr/lib/bluetooth/bluetoothd -C
Thêm dòng sau vào sau dòng ExecStart:

    ExecStartPost=/usr/bin/sdptool add SP
