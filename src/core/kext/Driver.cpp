#include <IOKit/IOLib.h>

#include "Core.hpp"
#include "Driver.hpp"
#include "IOLogWrapper.hpp"

// ----------------------------------------------------------------------
// http://developer.apple.com/documentation/DeviceDrivers/Conceptual/WritingDeviceDriver/CPluPlusRuntime/chapter_2_section_3.html

// This convention makes it easy to invoke base class member functions.
#define super    IOService
// You cannot use the "super" macro here, however, with the
//  OSDefineMetaClassAndStructors macro.
OSDefineMetaClassAndStructors(org_pqrs_driver_KeyRemap4MacBook, IOService)

// ----------------------------------------------------------------------
bool
org_pqrs_driver_KeyRemap4MacBook::init(OSDictionary* dict)
{
  IOLOG_INFO("init\n");

  bool res = super::init(dict);
  return res;
}

void
org_pqrs_driver_KeyRemap4MacBook::free(void)
{
  IOLOG_INFO("free\n");
  super::free();
}

IOService*
org_pqrs_driver_KeyRemap4MacBook::probe(IOService* provider, SInt32* score)
{
  IOService* res = super::probe(provider, score);
  return res;
}

bool
org_pqrs_driver_KeyRemap4MacBook::start(IOService* provider)
{
  IOLOG_INFO("start\n");

  bool res = super::start(provider);
  if (! res) return res;

  org_pqrs_KeyRemap4MacBook::Core::start();
  if (! initialize_notification()) return false;

  // Publish ourselves so clients can find us
  registerService();

  return res;
}

void
org_pqrs_driver_KeyRemap4MacBook::stop(IOService* provider)
{
  IOLOG_INFO("stop\n");

  terminate_notification();
  org_pqrs_KeyRemap4MacBook::Core::stop();

  super::stop(provider);
}


// ======================================================================
bool
org_pqrs_driver_KeyRemap4MacBook::initialize_notification(void)
{
  notifier_hookKeyboard_ = addMatchingNotification(gIOMatchedNotification,
                                                   serviceMatching("IOHIKeyboard"),
                                                   org_pqrs_KeyRemap4MacBook::Core::IOHIKeyboard_gIOMatchedNotification_callback,
                                                   this, NULL, 0);
  if (notifier_hookKeyboard_ == NULL) {
    IOLOG_ERROR("initialize_notification notifier_hookKeyboard_ == NULL\n");
    return false;
  }

  notifier_unhookKeyboard_ = addMatchingNotification(gIOTerminatedNotification,
                                                     serviceMatching("IOHIKeyboard"),
                                                     org_pqrs_KeyRemap4MacBook::Core::IOHIKeyboard_gIOTerminatedNotification_callback,
                                                     this, NULL, 0);
  if (notifier_unhookKeyboard_ == NULL) {
    IOLOG_ERROR("initialize_notification notifier_unhookKeyboard_ == NULL\n");
    return false;
  }

  // ----------------------------------------
  notifier_hookPointing_ = addMatchingNotification(gIOMatchedNotification,
                                                   serviceMatching("IOHIPointing"),
                                                   org_pqrs_KeyRemap4MacBook::Core::IOHIPointing_gIOMatchedNotification_callback,
                                                   this, NULL, 0);
  if (notifier_hookPointing_ == NULL) {
    IOLOG_ERROR("initialize_notification notifier_hookPointing_ == NULL\n");
    return false;
  }

  notifier_unhookPointing_ = addMatchingNotification(gIOTerminatedNotification,
                                                     serviceMatching("IOHIPointing"),
                                                     org_pqrs_KeyRemap4MacBook::Core::IOHIPointing_gIOTerminatedNotification_callback,
                                                     this, NULL, 0);
  if (notifier_unhookPointing_ == NULL) {
    IOLOG_ERROR("initialize_notification notifier_unhookPointing_ == NULL\n");
    return false;
  }

  return true;
}

void
org_pqrs_driver_KeyRemap4MacBook::terminate_notification(void)
{
  if (notifier_hookKeyboard_) notifier_hookKeyboard_->remove();
  if (notifier_unhookKeyboard_) notifier_unhookKeyboard_->remove();
  if (notifier_hookPointing_) notifier_hookPointing_->remove();
  if (notifier_unhookPointing_) notifier_unhookPointing_->remove();
}
