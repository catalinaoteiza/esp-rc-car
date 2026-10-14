# Roadmap

## ESP RC WiFi Car — Mecanum / Omnibot

This roadmap outlines potential improvements for the ESP-based WiFi-controlled robot platform. The goal is to evolve the current web joystick interface into a reliable, modular, and extensible robotics control system.

## Phase 1 — Core Stability and Reliability

**Goal:** Make the existing control system predictable, safe, and easy to maintain.

- [ ] **Improve connection handling**
  - Display clear WebSocket connection states.
  - Automatically reconnect after connection loss.
  - Stop all motors when the connection is lost.
  - Display connection errors and reconnection status.

- [ ] **Implement a control watchdog**
  - Stop the motors if control packets are not received within a configurable timeout.
  - Require a fresh control command after reconnection.
  - Ensure the robot starts with all motors disabled.

- [ ] **Standardize motor control**
  - Unify PWM/direction and dual-input H-bridge driver interfaces.
  - Centralize motor inversion and direction handling.
  - Validate forward, reverse, strafe, and rotation directions.
  - Add configurable speed limits and output clamping.

- [ ] **Improve the control interface**
  - Fix joystick resizing and orientation changes on mobile devices.
  - Improve status and enable/disable indicators.
  - Add a configurable dead zone and speed scaling.
  - Ensure the STOP control immediately sends a zero-motion command.

- [ ] **Improve configuration management**
  - Separate hardware pin assignments, motor settings, and network configuration.
  - Validate incoming WebSocket messages.
  - Document supported boards, drivers, and pin limitations.

## Phase 2 — Motion Control Improvements

**Goal:** Improve maneuverability and make the robot easier to operate.

- [ ] **Refine Mecanum kinematics**
  - Document the coordinate system and sign conventions.
  - Verify wheel mixing for all translation and rotation directions.
  - Normalize motor outputs to prevent command saturation.
  - Add automated tests for the motion-mixing calculations.

- [ ] **Add speed profiles**
  - Implement configurable acceleration and deceleration ramps.
  - Reduce abrupt starts and stops.
  - Allow independent translation and rotation speed limits.

- [ ] **Add control modes**
  - Tank/differential drive mode.
  - Omnidirectional Mecanum drive mode.
  - Rotation-only mode.
  - Optional precision mode for slow movements.

- [ ] **Improve servo control**
  - Make servo controls configurable and optionally visible in the UI.
  - Define minimum and maximum safe angles.
  - Support configurable center positions and movement speed.
  - Separate servo configuration from chassis movement commands.

## Phase 3 — Telemetry and Diagnostics

**Goal:** Make the robot's state observable from the web interface.

- [ ] **Add a telemetry protocol**
  - Define separate command and telemetry messages.
  - Include firmware version, enabled state, and connection status.
  - Reserve fields for battery voltage, motor output, and sensor readings.

- [ ] **Add battery monitoring**
  - Measure battery voltage using an appropriate sensor or ADC circuit.
  - Display voltage and estimated battery status in the UI.
  - Add configurable low-voltage warnings.
  - Implement low-voltage shutdown only after validating thresholds and hardware behavior.

- [ ] **Add runtime diagnostics**
  - Track uptime, packet frequency, and control timeouts.
  - Record errors and unexpected state transitions.
  - Provide a diagnostic page with firmware and hardware configuration details.

- [ ] **Add live telemetry charts**
  - Plot battery voltage and sensor measurements over time.
  - Visualize commanded motor outputs.
  - Provide a lightweight debugging mode.

## Phase 4 — Sensors and Feedback Control

**Goal:** Extend the platform beyond open-loop joystick control.

- [ ] **Integrate an IMU**
  - Support accelerometer and gyroscope readings.
  - Display orientation and angular velocity.
  - Evaluate motion stabilization and heading assistance.

- [ ] **Add wheel feedback**
  - Support wheel encoders where the motors and hardware permit.
  - Estimate wheel speed and distance traveled.
  - Compare commanded motion with measured motion.

- [ ] **Implement closed-loop control**
  - Add PID speed control when encoder feedback is available.
  - Compensate for differences between individual motors.
  - Evaluate heading-hold and straight-line driving assistance.

- [ ] **Add sensor abstraction layers**
  - Keep sensor drivers separate from the control logic.
  - Support optional hardware without requiring changes to the web interface.

## Phase 5 — User Experience and Advanced Control

**Goal:** Make the platform more useful for experimentation and everyday operation.

- [ ] **Add keyboard and gamepad support**
  - Control movement using keyboard shortcuts.
  - Support compatible USB or Bluetooth gamepads through the browser.
  - Include configurable bindings and a control-source indicator.

- [ ] **Add movement presets**
  - Save and replay predefined movement sequences.
  - Provide basic macros for translation and rotation.
  - Include execution limits and an emergency stop.

- [ ] **Improve the web interface**
  - Add a responsive dashboard for control, telemetry, and diagnostics.
  - Allow users to select control modes and adjust parameters.
  - Persist non-sensitive user preferences locally in the browser.

- [ ] **Add optional camera support**
  - Display a live video stream from a compatible camera or separate streaming device.
  - Evaluate bandwidth and latency before combining video with control traffic.

## Phase 6 — Architecture, Testing, and Documentation

**Goal:** Make the project easier to extend, test, and contribute to.

- [ ] **Modularize the firmware**
  - Separate networking, command parsing, motor drivers, kinematics, sensors, and safety logic.
  - Define stable interfaces between modules.
  - Keep hardware-specific code isolated.

- [ ] **Improve automated testing**
  - Unit-test motor mixing, normalization, and command parsing.
  - Test invalid packets, missing fields, and out-of-range values.
  - Add regression tests for direction and inversion behavior.
  - Document hardware-in-the-loop test procedures.

- [ ] **Improve build and release workflows**
  - Document PlatformIO environments for supported boards.
  - Automate firmware compilation and static checks.
  - Verify web assets are included in filesystem images.
  - Add version tags and release notes.

- [ ] **Expand documentation**
  - Provide wiring diagrams and supported hardware configurations.
  - Document network setup, flashing, and troubleshooting.
  - Explain the WebSocket protocol and control equations.
  - Add a contribution guide and hardware testing checklist.

## Phase 7 — Optional Advanced Features

**Goal:** Explore autonomous and networked robotics capabilities.

- [ ] **Add remote configuration**
  - Configure motor limits, inversion, and control parameters through a protected interface.
  - Validate settings before applying them.
  - Provide safe defaults and a reset-to-defaults option.

- [ ] **Explore autonomous navigation**
  - Add distance sensing using ultrasonic, ToF, or other compatible sensors.
  - Implement simple obstacle detection and assisted driving.
  - Explore waypoint navigation if suitable localization and feedback become available.

- [ ] **Evaluate ROS integration**
  - Investigate a bridge between the ESP firmware and a ROS-based host.
  - Publish telemetry and receive velocity commands through a suitable gateway.
  - Keep time-sensitive motor control and safety functions on the robot controller.

- [ ] **Explore multi-robot support**
  - Define a robot identification scheme.
  - Evaluate centralized control of multiple units.
  - Isolate command channels and safety states for each robot.

---

## Suggested Milestones

| Milestone | Scope | Completion criteria |
|---|---|---|
| M1 — Reliable Control | Phase 1 | Stable connection handling, watchdog, safe motor stop, validated directions |
| M2 — Smooth Movement | Phase 2 | Correct Mecanum mixing, output normalization, acceleration limits |
| M3 — Observable Robot | Phase 3 | Telemetry protocol, diagnostics, and battery monitoring |
| M4 — Feedback Control | Phase 4 | Sensor integration and at least one validated feedback-control feature |
| M5 — Better Interaction | Phase 5 | Keyboard/gamepad support and movement presets |
| M6 — Maintainable Project | Phase 6 | Modular firmware, automated tests, and complete setup documentation |
| M7 — Robotics Extensions | Phase 7 | One validated autonomous, ROS, or multi-robot feature |

## Recommended Initial Priorities

The following tasks offer the best starting point because they improve safety and reliability without requiring additional hardware:

1. Implement a communication watchdog and motor failsafe.
2. Verify and document all Mecanum movement directions.
3. Normalize motor commands and add acceleration ramps.
4. Improve WebSocket reconnection and connection-state handling.
5. Add automated tests for motion mixing and packet parsing.
6. Define a versioned command and telemetry protocol.

## Design Principles

- **Safety first:** A communication failure must not leave the robot moving indefinitely.
- **Modularity:** Motor drivers, sensors, networking, and motion control should be independently replaceable.
- **Hardware flexibility:** Optional sensors and peripherals should not be required for basic operation.
- **Backward compatibility:** Protocol changes should be documented and versioned.
- **Testability:** Core motion calculations and command parsing should be testable without physical hardware.
- **Incremental development:** Each milestone should leave the robot in a usable and documented state.

## Scope Notes

This roadmap describes potential future work, not features already implemented. Hardware-dependent features such as encoder feedback, battery monitoring, camera streaming, and autonomous navigation require compatible components and validation.

Priority should be reassessed as the hardware configuration and project goals evolve.