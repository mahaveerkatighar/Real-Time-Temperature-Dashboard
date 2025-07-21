# Real-Time Temperature Monitoring System by Mahaveer Katighar

This is a complete end-to-end IoT project that reads temperature from a DS18B20 sensor, sends the data to Google Firebase using an ESP8266, and visualizes the data on a live web dashboard.

This project was developed as part of my B.Tech studies to demonstrate practical skills in IoT, cloud integration, and web development.

![Live Dashboard Screenshot]()
*(To add your own screenshot, take a picture of your dashboard, upload it to a site like [Imgur](https://imgur.com/upload), and replace the link above)*

---

## Features
- **Real-Time Data:** The dashboard updates instantly as new sensor readings arrive from the ESP8266.
- **Cloud Integration:** Uses Google Firebase as a scalable, real-time, cloud-hosted database.
- **Web-Based Visualization:** A clean and simple web dashboard built with HTML, CSS, and Chart.js to provide a clear view of the temperature data over time.

---

## Technology Stack
- **Hardware:** ESP8266 (NodeMCU)
- **Sensor:** DS18B20 Digital Temperature Sensor
- **Firmware:** Programmed with Arduino C++
- **Database:** Google Firebase Realtime Database
- **Frontend:** HTML, CSS, JavaScript, and Chart.js

---

## How It Works
1.  The **ESP8266** microcontroller reads temperature data from the **DS18B20** sensor every few seconds.
2.  It connects to a local Wi-Fi network and uses an NTP server to get the accurate current time for timestamping.
3.  The timestamped temperature data (in both Celsius and Fahrenheit) is securely uploaded to the **Firebase Realtime Database**.
4.  The **web dashboard** (index.html) establishes a connection to Firebase and listens for any changes in the database. When new data is added, it automatically updates the chart in real-time using **Chart.js**.

