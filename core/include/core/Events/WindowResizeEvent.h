#pragma once
#include <sstream>

#include "Event.h"
#include "pch.h"

namespace vr {
class WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(u32 width, u32 height) : m_W(width), m_H(height) {}
  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
  u32 GetWidth() const { return m_W; }
  u32 GetHeight() const { return m_H; }
  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << m_W << ", " << m_H;
    return ss.str();
  }

 private:
  u32 m_W, m_H;
};
class WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() = default;
  EVENT_CLASS_TYPE(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}  // namespace vr
