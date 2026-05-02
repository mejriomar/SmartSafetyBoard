# SmartSafetyBoard

A comprehensive safety dashboard system built on ESP32 that displays real-time safety metrics, weather information, and time data on an LED matrix display with a web-based management interface.

## 📋 Overview

SmartSafetyBoard is an embedded IoT solution designed for workplaces and industrial environments to monitor and display safety metrics including:
- Accident-free days counter
- Real-time clock and date
- Current weather and temperature
- Safety status indicators
- IP address and network information

The system features both a physical LED matrix display (3x DMD32) and a web interface for remote monitoring and configuration.

## 🎯 Features

### Hardware Display
- **LED Matrix Display**: 3 DMD32 LED dot matrix modules in a 3×1 configuration
- **Real-time Refresh**: Hardware timer-driven display scanning at configurable refresh rates
- **Multiple Display Pages**: Automatic cycling between different information views

### Web Interface
- **Responsive Dashboard**: Access safety metrics from any web browser
- **Login Authentication**: Secure access with username and password protection
- **Settings Management**: Configure:
  - Accident-free days counter
  - Record/Historical data
  - City for weather information
  - API keys for data services
  - Registration/Startup time
  - Access point status
- **Live Data Display**: Real-time updates of all system metrics

### Connectivity
- **WiFi Capability**: Dual-mode WiFi (AP and STA)
  - Access Point (AP): Create a local network for direct connection
  - Station (STA): Connect to existing WiFi networks
- **Auto WiFi Configuration**: Built-in WiFi settings management via WiFiSettings library
- **Network Information**: Display IP address and connection status

### Data Integration
- **OpenWeatherMap API**: Real-time weather data fetching
  - Temperature display
  - Location-based weather
  - Configurable city and country
- **Network Time Protocol (NTP)**: Accurate time synchronization
  - Time display (HH:MM format)
  - Date display (DD/MM/YYYY format)
  - Day of week
  - Timezone support (configurable UTC offset)

### Data Persistence
- **EEPROM Storage**: Persistent data retention including:
  - Accident-free days counter
  - Record values
  - Selected city for weather
  - OpenWeatherMap API key
- **Automatic Recovery**: Configuration survives power cycles and restarts

## 🔧 Hardware Requirements

- **Microcontroller**: ESP32 Development Board (e.g., ESP32-DevKitC)
- **Display**: 3× DMD32 LED Dot Matrix Modules (P4 or P5 modules)
- **Power Supply**: 5V/2A minimum for LED modules + ESP32
- **Connections**: SPI interface for display modules

## 📦 Dependencies

All dependencies are specified in `platformio.ini`:

```
Platform: Espressif32
Board: esp32dev
Framework: Arduino
Libraries:
  - AsyncTCP (v1.1.1+)
  - ESPAsyncWebServer-esphome (v3.1.0+)
  - ESP-WiFiSettings (v3.8.0+)
  - DMD32 (for LED matrix display)
  - Arduino_JSON (for JSON parsing)
  - HTTPClient (for API requests)
```

## 🚀 Getting Started

### Prerequisites
- PlatformIO IDE or CLI installed
- ESP32 board connected to your computer
- Internet connection for initial setup

### Installation

1. **Clone or download the repository:**
   ```bash
   git clone [your-repository-url]
   cd SmartSafetyBoard
   ```

2. **Open the project in PlatformIO:**
   ```bash
   platformio open --project-dir .
   ```

3. **Configure the environment** (optional):
   - Edit `platformio.ini` to specify your board type if using a variant other than esp32dev

4. **Build the project:**
   ```bash
   platformio run
   ```

5. **Upload to ESP32:**
   ```bash
   platformio run --target upload
   ```

6. **Monitor serial output:**
   ```bash
   platformio device monitor
   ```

## ⚙️ Configuration

### WiFi Setup

The device automatically enters WiFi configuration mode on first boot or when needed:

1. Look for network named **"TABLEAU"** in your available networks
2. Connect to this network (no password required)
3. Open a browser and navigate to the device's IP address
4. Follow the WiFiSettings configuration interface
5. The device will connect to your configured WiFi network

### Web Interface Access

1. **Default Access Point**: 
   - SSID: `TABLEAU`
   - No password
   - Access at: `http://192.168.4.1/` (AP mode)

2. **After WiFi Configuration**:
   - Access at: `http://[device-ip-address]/`
   - Default credentials: 
     - Username: `omar`
     - Password: `1234`
     - **⚠️ Change these credentials in code before production use**

### API Configuration

1. **Get OpenWeatherMap API Key**:
   - Visit [openweathermap.org](https://openweathermap.org/api)
   - Create a free account and generate an API key
   - Keep the key handy

2. **Configure via Web Interface**:
   - Log in to the dashboard
   - Enter the API key in the settings
   - Select your city and country
   - Save settings (data is persisted to EEPROM)

### Parameters

Key configurable parameters in the web interface:

| Parameter | Description | Default |
|-----------|-------------|---------|
| Accident-free Days | Current streak of accident-free days | 0 |
| Record | Historical record of accident-free days | 0 |
| City | City name for weather API queries | ben arous |
| Country Code | Country code (ISO 2-letter) | TN |
| Registration Time | System startup/registration time | - |
| API Key | OpenWeatherMap API key | (none) |
| Access Point | Toggle AP mode ON/OFF | ON |

## 🌐 Web Interface Endpoints

### API Endpoints

| Endpoint | Method | Purpose |
|----------|--------|---------|
| `/` | GET | Login page / Main dashboard |
| `/api` | GET | Return API status/information |
| `/ip` | GET | Return device IP address |
| `/get1` | GET | Update dashboard settings |
| `/get2` | GET | System control (restart, mode changes) |
| `/affich` | GET | Display page with all metrics |

### Query Parameters for `/get1`

- `input1`: Accident-free days counter
- `input2`: Record value
- `input3`: Registration time
- `input7`: City for weather
- `apikey`: OpenWeatherMap API key

## 📊 Display Information

The LED matrix displays the following metrics on rotation:

1. **Status**: Current system state (ON/OFF)
2. **Time**: Current time (HH:MM format)
3. **Date**: Current date (DD/MM/YYYY)
4. **Day of Week**: Current day name
5. **Registration Time**: System startup time
6. **Temperature**: Current temperature from weather API
7. **City**: Selected city for weather data
8. **Days Without Accident**: Current accident-free streak
9. **Record**: Historical accident-free record
10. **IP Address**: Device IP address
11. **Access Point**: AP mode status (ON/OFF)

## 🔐 Security Considerations

⚠️ **Important**: The default credentials are hardcoded for demonstration purposes.

**Before deployment:**
1. Change the default username (`omar`) and password (`1234`) in code
2. Implement proper authentication mechanism
3. Use HTTPS for web interface (requires SSL certificate)
4. Restrict access to trusted networks
5. Regularly update API keys and credentials
6. Consider implementing token-based authentication

## 🛠️ Development

### Project Structure
```
SmartSafetyBoard/
├── src/
│   └── main.cpp          # Main application code
├── include/
│   └── README            # Include directory info
├── lib/
│   └── README            # Library directory info
├── test/
│   └── README            # Test directory info
├── platformio.ini        # Project configuration
└── README.md             # This file
```

### Key Components

- **Web Server**: AsyncWebServer for non-blocking HTTP handling
- **Display Driver**: DMD32 library for LED matrix control
- **WiFi Manager**: WiFiSettings for easy network configuration
- **Storage**: EEPROM for persistent data
- **Time Sync**: NTP client for accurate timekeeping
- **Weather API**: OpenWeatherMap integration via HTTPClient

### Customization

To modify the display layout:
- Edit the `affich[]` HTML template in `main.cpp` (around line 200+)
- Modify CSS styles for appearance
- Update the display rotation logic in the `loop()` function

## 📡 Serial Communication

The device outputs debug information on the serial port (115200 baud):

```bash
# Monitor serial output
platformio device monitor --baud 115200
```

Expected output on startup:
- Initialization messages
- WiFi connection status
- IP address assignment
- EEPROM status
- API key status

## 🐛 Troubleshooting

### No WiFi Connection
- Check WiFi credentials
- Verify router is within range
- Look for "TABLEAU" network in AP mode
- Check serial monitor for connection messages

### Display Not Updating
- Verify DMD32 library is installed
- Check SPI pin connections
- Ensure power supply is adequate for all 3 displays
- Monitor for timer interrupt issues in serial output

### Weather Data Not Updating
- Verify OpenWeatherMap API key is set
- Check internet connectivity
- Confirm city name is correct
- Verify API key has active balance

### Cannot Access Web Interface
- Verify device IP address from serial monitor
- Ensure you're connected to correct WiFi network
- Try accessing via AP mode first
- Check firewall settings

### EEPROM Error
- Device will auto-restart if EEPROM fails
- Check serial output for specific errors
- May indicate hardware issue

## 📝 License

[no license]

## 👨‍💻 Author

[Omar Mejri]

## 📧 Support

For issues, questions, or contributions, please:
- Check existing issues in the repository
- Create a detailed issue report with:
  - Serial output logs
  - Steps to reproduce
  - Expected vs actual behavior
  - Hardware configuration

## 🗺️ Roadmap

Potential future enhancements:
- [ ] Mobile app for notifications
- [ ] Historical data logging and analytics
- [ ] Multiple safety metrics tracking
- [ ] Alert system for safety violations
- [ ] Integration with incident reporting systems
- [ ] MQTT support for IoT ecosystems
- [ ] Over-the-air (OTA) firmware updates
- [ ] Advanced authentication (OAuth, JWT)
- [ ] Multi-language support
- [ ] Data export functionality

## 📚 References

- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [AsyncWebServer Library](https://github.com/me-no-dev/ESPAsyncWebServer)
- [OpenWeatherMap API](https://openweathermap.org/api)
- [DMD32 Library](https://github.com/MariusvanWijk/DMD32)
- [WiFiSettings Library](https://github.com/Juerd/ESP-WiFiSettings)

---

**Last Updated**: May 2, 2026
