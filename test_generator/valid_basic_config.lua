local consts = require 'lua/ab_consts'

return {
  TestType = {
    type = "list",
    values = {"ABTest", "XYTest"}
  },
  BinType = {
    type = "list",
    values = {"anonymous", "c_to_v_ok_v_to_c_ok_v_to_v_not_ok", } -- TODO add more later
  },
  description = {
    type = "regex",
    length = {
      random = {
        lower = "1",
        upper = "4000"
      }
    },
    regex = "%w-_"
  },
  Creator = {
    type = "regex",
    length = {
      random = {
        lower = "1",
        upper = "4000"
      }
    },
    regex = "%w-_",
  },
  name = {
    type = "regex",
    length = {
      random = {
        lower = "1",
        upper = tostring(consts.AB_MAX_LEN_VARIANT_NAME)
      }
    },
    regex = "%w"
  },
  id = {
    type = "number",
    value = {
      random = {
        lower = "0",
        upper = "214748"
      }
    }
  },
  State = {
    type = "list",
    values = {"archived", "terminated", "started"}
  },
  seed = {
    type = "number",
    value = {
      random = {
        lower = "0",
        upper = "18446744"
      }
    }
  },
  is_dev_specific = {
    type = "list",
    values = {"0", "1"}
  },
  Variants = {
    type = "set",
    constraints = {
      percentage = {
        type = "sum",
        value = "100",
      },
      id = {
        type = "unique"
      },
    },
    entry_fields = {
      id = {
        type = "number",
        value = {
          random = {
            lower = "0",
            upper = "214748",
          }
        },
      },
      name = {
        type = "regex",
        length = {
          random = {
            lower = "1",
            upper = tostring(consts.AB_MAX_LEN_VARIANT_NAME),
          },
        },
        regex = "%w",
      },
      percentage = {
        type = "number",
        float = true, 
        value = {
          random = {
            lower = "0",
            upper = "100",
          },
        },
      },
      url = {
        type = "regex",
        length = {
          random = {
            lower = "1",
            upper = tostring(consts.AB_MAX_LEN_VARIANT_NAME),
          },
        },
        regex = "%w",
      },
    },
  },
}
