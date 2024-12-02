# WiFi Audio Dongle

A real-time audio streaming application built in C++ that captures audio from selected Windows applications using WASAPI, detaches it from system speakers, and streams it over WiFi to a Linux receiver for playback. This project aims to provide a lightweight, open-source alternative to hardware audio dongles, enabling users to stream audio flexibly.

## Project Status

Currently, development is focused on:
- **Sender**: Windows-based application to capture and stream audio.
- **Receiver**: Linux-based application to receive and play audio streams.

### Future Plans
We plan to make both the **Sender** and **Receiver** cross-platform, allowing seamless audio streaming between different operating systems. Contributions toward this goal are highly encouraged!

## Contributing

We welcome contributions from the community! If you'd like to help develop this project, here’s how you can contribute:
1. **Fork** the repository.
2. **Develop** your feature or fix in a separate branch.
3. **Submit a Pull Request (PR)** to the `development` branch with a clear description of your changes.

Areas where contributions are especially helpful:
- Cross-platform compatibility for both sender and receiver.
- Optimizations for low-latency streaming.
- Bug fixes and performance improvements.

## License

This project is licensed under the **GNU Affero General Public License v3.0 (AGPL-3.0)**. See the [LICENSE](LICENSE) file for details.

## Disclaimer

The code is still in development, and no functional release is available yet. A "Getting Started" guide will be added once the code is ready for use.

---

Thank you for your interest in contributing to Audio Dongle Over WiFi! Together, we can create a powerful, flexible, and open-source audio streaming solution.