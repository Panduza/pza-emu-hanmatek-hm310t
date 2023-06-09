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
    

    # --- GETS ---

    def read_psu_model(self):
        content = self.master.read_holding_registers(address=0x03, count=1, slave=0x01)
        content = content.registers[0]
        print("PSU model = " + str(content))
        return content
    
    def read_psu_class(self):
        content = self.master.read_holding_registers(address=0x04, count=1, slave=0x01)
        content = content.registers[0]
        print("PSU class = " + str(content))
        return content

    def read_voltage_goal(self):
        voltageGoal = self.master.read_holding_registers(address=0x30, count=1, slave=0x01)
        voltageGoal = voltageGoal.registers[0]
        print("Voltage goal = " + str(voltageGoal))
        return voltageGoal
    
    def read_voltage_real(self):
        voltageReal = self.master.read_holding_registers(address=0x10, count=1, slave=0x01)
        voltageReal = voltageReal.registers[0]
        print("Voltage real = " + str(voltageReal))
        return voltageReal
        
    def read_amps_goal(self):
        ampsGoal = self.master.read_holding_registers(address=0x31, count=1, slave=0x01)
        ampsGoal = ampsGoal.registers[0]
        print("Amps goal = " + str(ampsGoal))
        return ampsGoal
        
    def read_amps_real(self):
        ampsReal = self.master.read_holding_registers(address=0x11, count=1, slave=0x01)
        ampsReal = ampsReal.registers[0]
        print("Amps real = " + str(ampsReal))
        return ampsReal
        
    def read_enable(self):
        enable = self.master.read_holding_registers(address=0x01, count=1, slave=0x01)
        enable = enable.registers[0]
        print("Enable status = " + str(enable))
        return enable
    
    def read_power_consumption(self):
        powerH = self.master.read_holding_registers(address=0x12, count=1, slave=0x01)
        powerL = self.master.read_holding_registers(address=0x13, count=1, slave=0x01)
        powerH = powerH.registers[0]
        powerL = powerL.registers[0]
        power = (powerH << 16) + powerL
        print("Power consumption L = " + str(powerL))
        print("Power consumption H = " + str(powerH))
        print("Power consumption = " + str(power))
        return power
        

    # --- SET ---

    def write_psu_model(self, value):
        sending = self.master.write_registers(0x03, int(value), 0x01)

    def write_voltage_goal(self, voltage):
        sending = self.master.write_registers(0x30, int(voltage), 0x01)

    def write_voltage_real(self, voltage):
        """ Illegal action
        """
        sending = self.master.write_registers(0x10, int(voltage), 0x01)

    def write_amps_goal(self, amps):
        sending = self.master.write_registers(0x31, int(amps), 0x01)

    def write_enable(self, state):
        sending = self.master.write_registers(0x1, int(state) , 0x01)

    def turn_on_power_supply(self):
        self.write_enable(1)

    def turn_off_power_supply(self):
        self.write_enable(0)


    # --- CHECK ---

    def psu_model_should_be(self, expected):
        modelReal = self.read_psu_model()
        if modelReal != int(expected):
            raise AssertionError('%s != %s' % (modelReal, int(expected)))

    def psu_class_should_be(self, expected):
        classReal = self.read_psu_class()
        if classReal != int(expected):
            raise AssertionError('%s != %s' % (classReal, int(expected)))

    def voltage_real_should_be(self, voltageExpected):
        voltageReal = self.read_voltage_real()
        if voltageReal != int(voltageExpected):
            raise AssertionError('%s != %s' % (voltageReal, int(voltageExpected)))
    
    def voltage_goal_should_be(self, voltageExpected):
        voltageGoal = self.read_voltage_goal()
        if voltageGoal != int(voltageExpected):
            raise AssertionError('%s != %s' % (voltageGoal, int(voltageExpected)))
        
    def amps_real_should_be(self, apmsExpected):
        ampsReal = self.read_amps_real()
        if ampsReal != int(apmsExpected):
            raise AssertionError('%s != %s' % (ampsReal, int(apmsExpected)))

    def amps_goal_should_be(self, apmsExpected):
        ampsGoal = self.read_amps_goal()
        if ampsGoal != int(apmsExpected):
            raise AssertionError('%s != %s' % (ampsGoal, int(apmsExpected)))

    def enable_should_be(self, enableExpected):
        enableReal = self.read_enable()
        if enableReal != int(enableExpected):
            raise AssertionError('%s != %s' % (enableReal, int(enableExpected)))
        
    def power_supply_should_be(self, enableExpected):
        state = {"on": "1", "off": "0"}
        self.enable_should_be(state[enableExpected])


if __name__ == '__main__':
    interface = Client()
    voltageReal = interface.read_voltage_real()
    print("***"+str(voltageReal)+"***")
    print(voltageReal)
    print(type(voltageReal))

    print(interface.read_amps_real())
    print(interface.read_amps_goal())

    
