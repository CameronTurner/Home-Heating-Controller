# Home-Heating-Controller
Code to run Particle.io device to control a heater via a relay device

This code has three features:
  1. Heater On
  2. Heater On until the target temperature is reached
  3. Heater Off


This code offers three functions, in detail:
  1. Signal a relay to complete the circuit to activate a home heater
  2. Signal a relay to complete the circuit to activate a home heater - only up until a set temperature. 
  The temperature information comes from another Particle Photon device that has a DHT sensor.
  3. Signal a relay to turn off.
