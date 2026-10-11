#pragma once
#include <memory>
#include <thread>

namespace cemuvr {
// Listens on a Windows recording device for blowing. Audio stays in memory,
// is analysed in 10 ms blocks by MicBlowDetector and then discarded; nothing
// is recorded, stored or sent.
//
// The device is opened only while the game checks for blowing: the blow
// hooks count the blow checks of game objects in a level, the layer reports each change
// through noteDemand(), and five seconds without a check close the device
// again. Before that - title, menus, levels without blowing - it stays closed.
//
// CEMUVR_BLOW_MIC     unset or "1": the default recording device
//                     "0": off
//                     other text: first active recording device whose name
//                     contains it (case-insensitive)
// CEMUVR_BLOW_MIC_DB  quietest level that can count as blowing, in dBFS
//                     (default -45; lower is more sensitive)
class MicBlowCapture {
public:
    MicBlowCapture() = default;
    ~MicBlowCapture() { stop(); }
    MicBlowCapture(const MicBlowCapture&) = delete;
    MicBlowCapture& operator=(const MicBlowCapture&) = delete;

    void start();          // starts the worker; no device is opened yet
    void stop();           // waits at most 500 ms, never on a hanging audio driver
    void noteDemand();     // the game queried the microphone just now
    bool blowing() const;

    struct State;          // shared with the worker, so it outlives a late worker
private:
    std::shared_ptr<State> m_state;
    std::thread m_thread;
};
} // namespace cemuvr
