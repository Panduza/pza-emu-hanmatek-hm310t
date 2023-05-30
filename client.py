from panduza import Client, Psu
import time

# CONFIGURATION
BROKER_ADDR="localhost"
BROKER_PORT=1883

# Do not forget to remplace the IP and PORT by the one you target
pzaTOPIC_OUT=f"pza/rename_this_machine/hanmatek.hm310t/test_alim_quentin/atts"
c = Client(url=BROKER_ADDR, port=BROKER_PORT)

# Then connect and scan
c.connect()

# list all the topics to find the PSU
print("scanning the interfaces..")
inter = c.scan_interfaces()
for topic in inter:
    print(f"- {topic} => {inter[topic]['type']}")
    if inter[topic]['type'] == "psu" :
        my_psu = Psu(addr=BROKER_ADDR, port=BROKER_PORT, topic=topic, client=c)


# ***   VOLTAGE   ***

#Write
voltage_goal=3
print("\nSet voltage to " + str(voltage_goal))
my_psu.volts.goal.set(voltage_goal)
time.sleep(2)
#Read
voltage_real=my_psu.volts.real.get()
print("Voltage real is " + str(voltage_real))
time.sleep(2)

#Write
voltage_goal=10
print("\nSet voltage to " + str(voltage_goal))
my_psu.volts.goal.set(voltage_goal)
time.sleep(2)
#Read
voltage_real=my_psu.volts.real.get()
print("Voltage real is " + str(voltage_real))
time.sleep(2)


# ***   AMPS   ***

#Write
amps_goal=0.1
print("\nset amps to " + str(amps_goal))
my_psu.amps.goal.set(amps_goal)
time.sleep(2)
#Read
print("Get amps...")
amps_real=my_psu.amps.real.get()
print("amps real is " + str(amps_real))
time.sleep(2)

#Write
amps_goal=0.5
print("\nset amps to " + str(amps_goal))
my_psu.amps.goal.set(amps_goal)
time.sleep(2)
#Read
amps_real=my_psu.amps.real.get()
print("amps real is " + str(amps_real))
time.sleep(2)

var="n"
print("Do you want to set enable->value (unsafe) ? (y/n)")
var=input()
if(var != "y" ):
    print("Skip setting enable->value")
    exit()
# ***   POWER ON/OFF   ***
#Write
state=True
print("\nset state to " + str(state))
my_psu.enable.value.set(True)
time.sleep(2)
#Read
print("Get state...")
state=my_psu.enable.value.get()
print("State is " + str(state))
time.sleep(2)

#Write
state=False
print("\nset state to " + str(state))
my_psu.enable.value.set(False)
time.sleep(2)
#Read
print("Get state...")
state=my_psu.enable.value.get()
print("State is " + str(state))