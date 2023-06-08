# -*- Coding: utf-8 -*-
from pymodbus.client import ModbusSerialClient
from pymodbus.transaction import ModbusRtuFramer
import time
import logging

class Client:

    def __init__(self):
        self.master = ModbusSerialClient(framer=ModbusRtuFramer, port = '/dev/ttyACM0', stopbits=1, bytesize=8, parity='N', baudrate=115200)
        connexion = self.master.connect()
        print(self.master)
        print("\033[92m-------------------client connected----------------------\033[0m")
        
    def mon_keyword_perso(self, texte):
        print("voici comment imprimer dans le log.html")
        return "le texte etait: %s" % (texte)
    
    def read_voltage_goal(self):
        voltageGoal = self.master.read_holding_registers(address=0x30, count=1, slave=0x01)
        return voltageGoal.registers[0]
    
    def read_voltage_real(self):
        voltageReal = self.master.read_holding_registers(address=0x10, count=1, slave=0x01)
        return voltageReal.registers[0]
        
    def read_amps_goal(self):
        ampsGoal = self.master.read_holding_registers(address=0x31, count=1, slave=0x01)
        return ampsGoal.registers[0]
        
    def read_amps_real(self):
        ampsReal = self.master.read_holding_registers(address=0x11, count=1, slave=0x01)
        return ampsReal.registers[0]
        
    def read_enable(self):
        enable = self.master.read_holding_registers(address=0x01, count=1, slave=0x01)
        return enable.registers[0]
        
    def write_voltage_goal(self, voltage):
        sending = self.master.write_registers(0x30, int(voltage), 0x01)

    def write_amps_goal(self, amps):
        sending = self.master.write_registers(0x31, int(amps), 0x01)

    def write_enable(self, state):
        sending = self.master.write_registers(0x1, int(state) , 0x01)

    def voltage_real_should_be(self, voltageExpected):
        voltageReal = self.read_voltage_real()
        if voltageReal != int(voltageExpected):
            raise AssertionError('%s != %s' % (voltageReal, int(voltageExpected)))

if __name__ == '__main__':
    interface = Client()
    voltageReal = interface.read_voltage_real()
    print("***"+str(voltageReal)+"***")
    print(voltageReal)
    print(type(voltageReal))

    
