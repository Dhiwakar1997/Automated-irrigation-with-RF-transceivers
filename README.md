# Automated-irrigation-with-RF-transceivers
This is an agriculture-based project which automates irrigation.

This project is of two phases:

Moisture Sensor Setup - This setup is installed in the field. 
                          The moisture sensor is immersed in the soil to measure the soil's moisture level.
                          This moisture percentage is transferred wirelessly using NRF24L01 transceiver.

2. Motor controller - This setup receives the moisture data from the moisture sensor setup. Based on the 
                    moisture percentage of the soil this setup will turn On/Off the motor pump using a relay. 
