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
voltage_goal=1
print("\nset voltage to " + str(voltage_goal))
my_psu.volts.goal.set(voltage_goal)

voltage_real=my_psu.volts.real.get()
print("voltage real is " + str(voltage_real))

time.sleep(2)

voltage_goal=2
print("\nset voltage to " + str(voltage_goal))
my_psu.volts.goal.set(voltage_goal)

voltage_real=my_psu.volts.real.get()
print("voltage real is " + str(voltage_real))
time.sleep(2)


# ***   AMPS   ***

#Write
amps_goal=0.1
print("\nset amps to " + str(amps_goal))
my_psu.amps.goal.set(amps_goal)
#Read
print("Get amps...")
amps_real=my_psu.amps.real.get()
print("amps real is " + str(amps_real))
time.sleep(2)

#Write
amps_goal=0.05
print("\nset amps to " + str(amps_goal))
my_psu.amps.goal.set(amps_goal)
#Read
amps_real=my_psu.amps.real.get()
print("amps real is " + str(amps_real))
time.sleep(2)

# +++ TEST
# ***   POWER ON/OFF   ***

print("\nGet state...")
state=my_psu.enable.value.get()
print("State is " + str(state))
time.sleep(2)

print("\nSet state to 'true'")
#my_psu.enable.value.set("true") #ne marche pas
#my_psu.enable.value.set(1) #ne marche pas
my_psu.enable.value.set(True) # marche

#my_psu.enable.value.set(False) # ne marche pas : Fail to set enable.value
#my_psu.enable.value.set(0) # ne marche pas : err type

print("\nGet state...")
state=my_psu.enable.value.get()
print("State is " + str(state))
time.sleep(2)
exit()
# +++ END TEST

# ***   POWER ON/OFF   ***
#Write
state=True
print("\nset state to " + str(state))
my_psu.enable.value.set(True)
#Read
print("Get state...")
state=my_psu.enable.value.get()
print("State is " + str(state))
time.sleep(2)

#Write
state=False
print("\nset state to " + str(state))
my_psu.enable.value.set(False) #ne marche pas
#Read
print("Get state...")
state=my_psu.enable.value.get()
print("State is " + str(state))