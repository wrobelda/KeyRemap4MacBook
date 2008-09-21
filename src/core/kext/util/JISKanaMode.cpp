#include "JISKanaMode.hpp"
#include "CallBackWrapper.hpp"
#include "FlagStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  extern AllFlagStatus allFlagStatus;

  namespace JISKanaMode {
    Mode mode;

    Mode
    getMode(void)
    {
      return mode;
    }

    void
    setMode(Mode newmode)
    {
      mode = newmode;
    }

    void
    setMode(const Params_KeyboardEventCallBack &params)
    {
      if (params.eventType != KeyEvent::DOWN) return;

      if (params.key == KeyCode::JIS_EISUU) {
        setMode(JISKANAMODE_ASCII);

      } else if (params.key == KeyCode::JIS_KANA) {
        if (allFlagStatus.isHeldDown_shift()) {
          setMode(JISKANAMODE_KATAKANA);
        } else {
          setMode(JISKANAMODE_HIRAGANA);
        }
      }
    }

    void
    toggle(void)
    {
      if (mode == JISKANAMODE_ASCII) {
        if (allFlagStatus.isHeldDown_shift()) {
          setMode(JISKANAMODE_KATAKANA);
        } else {
          setMode(JISKANAMODE_HIRAGANA);
        }
      } else {
        setMode(JISKANAMODE_ASCII);
      }
    }

    bool
    iskana(void)
    {
      return mode != JISKANAMODE_ASCII;
    }
  }
}
