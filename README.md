# Practice Pedal :zap::guitar:

The Practice Pedal project was completed during **HackGT9 Retro Reset** and won the **Best Hardware Hack 🛠️ Award**! Check out our [Devpost](https://devpost.com/software/practice-pedals).

This project focuses on repurposing old guitar pedals into gamified devices to motivate kids to practice and get passionate about playing the electric guitar. 

1. We've done this by reading a voltage generated from the electric guitar into our pedal using signal processing (with an op-amp and full-wave rectifier that we wired to simplify playtime tracking), and using that data to track how long a player has been actively using the guitar while connected into our pedal with a connected Arduino Nano. 
![dispenser](resources/repurposed_guitarpedal.jpg)
3. This Arduino Nano then communicates via a custom heat-shrinked cable to a dispensing device that we built. 
4. The dispenser device can be loaded up with starbursts, and each time a player hits a milestone, it has a central servo motor that is activated, dropping a single starbust through our mechanism. The dispenser device also has an LCD display, which we use to show time-elapsed playing and the goal time. This LCD also displays a "Congrats!" screen upon hitting the milestone, along with a custom music note character that we programmed in. 

![dispenser](resources/final_dispenserproto.jpg)



