#ifndef _JOYBUS_N64CONSOLE_HPP
#define _JOYBUS_N64CONSOLE_HPP

#include "n64_definitions.h"
#include "joybus.h"

#include <hardware/pio.h>
#include <pico/stdlib.h>

enum class PollStatus_N64 {
    RUMBLE_OFF,
    RUMBLE_ON,
    ERROR,
};

class N64Console {
  public:
    /**
     * @brief Construct a new N64Console object
     *
     * @param pin The GPIO pin that the N64 console's data line is connected to
     * @param pio The PIO instance; either pio0 or pio1. Default is pio0.
     * @param sm The PIO state machine to run the joybus instance on. Default is to automatically
     * claim an unused one.
     * @param offset The instruction memory offset at which to load the PIO program. Default is to
     * allocate automatically.
     */
    N64Console(uint pin, PIO pio = pio0, int sm = -1, int offset = -1);

    /**
     * @brief Cleanly terminate the joybus PIO instance, freeing the state machine, and uninstalling
     * the joybus program from the PIO instance
     */
    ~N64Console();

    /**
     * @brief Detect if a N64 console is connected to the joybus port
     *
     * @return true if console is detected, false otherwise
     */
    bool Detect();

    /**
     * @brief Block until a poll is received from the N64 console. Automatically responds to
     * any probe/origin commands received in the process.
     *
     * @return true if rumble bit is high, false otherwise
     */
    bool WaitForPoll();

    /**
     * @brief Block until the first byte of a poll command is received from the N64 console.
     * Automatically responds to any probe/origin commands received in the process. This function is
     * provided in order to make it possible to do processing while the second and third bytes of
     * the poll command. You must call WaitForPollEnd() before SendReport() if using this function.
     *
     * @return The reading mode requested by the poll command
     */
    void WaitForPollStart();

    /**
     * @brief Block until last two bytes of a poll command are received, or time out after 50us.
     * Must be called before SendReport() if you called WaitForPollStart().
     *
     * @return PollStatus enum indicating RUMBLE_OFF, RUMBLE_ON, or ERROR
     */
    PollStatus_N64 WaitForPollEnd();

    /**
     * @brief Send a N64 controller input report to a connected N64 console
     *
     * @param report The report to send
     */
    void SendReport(n64_report_t *report);

    /**
     * @brief Get the offset at which the PIO program was installed. Useful if you want to
     * communicate with multiple joybus devices without having to load multiple copies of the PIO
     * program.
     *
     * @return The offset at which the PIO program was installed
     */
    int GetOffset();

  private:
    static constexpr uint incoming_bit_length_us = 5;
    static constexpr uint max_command_bytes = 1;
    static constexpr uint receive_timeout_us = incoming_bit_length_us * 10;
    static constexpr uint reset_wait_period_us =
        (incoming_bit_length_us * 8) * (max_command_bytes - 1) + receive_timeout_us;
    static constexpr uint64_t reply_delay = incoming_bit_length_us - 1;

    joybus_port_t _port;
    absolute_time_t _receive_end;
    uint8_t _reading_mode = 3;
};

#endif
