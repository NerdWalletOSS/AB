package.path=package.path .. ";./../src/?.lua"
local consts = require 'ab_consts'

return {
  id = {
    type = "number",
    value = {
      random = {
        lower = "0",
        upper = "214748"
      }
    }
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
  api_id = {
    --TODO Ranesh double check
    type = "regex",
    length = {
      random = {
        lower = "1",
        upper = "4"
      }
    },
    regex = "%d"
  },
  request_webapp_id =  {
    --TODO Ranesh double check{
    type = "regex",
    length = {
      random = {
        lower = "1",
        upper = "40"
      }
    },
    regex = "%w-_"
  },
  test_type_id = {
    --TODO Ranesh double check
    type = "regex",
    length = {
      random = {
        lower = "1",
        upper = "4"
      }
    },
    regex = "%d"
  },
  bin_type_id = {
    --TODO Ranesh double check
    type = "regex",
    length = {
      random = {
        lower = "1",
        upper = "4"
      }
    },
    regex = "%d"
  },
  pred_id = {
    --TODO Ranesh double check
    type = "list",
    values = {"null"}
  },
  channel_id = {
    --TODO Ranesh double check
    type = "list",
    values = {"null"}
  },
  description = {
    --TODO Ranesh double check
    type = "regex",
    length = {
      random = {
        lower = "1",
        upper = "40"
      }
    },
    regex = "%w-_"
  },
  is_dev_specific = {
    --TODO Ranesh double check
    type = "list",
    values = {"0", "1"}
  },
  has_filters = {
    --TODO Ranesh double check
    type = "list",
    values = {"0"}
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
  external_id = {
    type = "number",
    value = {
      random = {
        lower = "0",
        upper = "18446744"
      }
    }
  },
  -- "created_at": "2018-02-05 15:35:39",
  -- "t_create": "151787373911388600",
  -- "updated_at": "2018-02-05 15:35:39",
  -- "t_update": "151787373913040700",
  -- "creator_id": "1",
  -- "updater_id": "1",
  -- "state_id": "1",
  -- "Creator": "joe",
  TestType = {
    type = "list",
    values = {"ABTest", "XYTest"}
  },
  BinType = {
    type = "list",
    values = {"anonymous", "c_to_v_ok_v_to_c_ok_v_to_v_not_ok", } -- TODO add more later
  },
  State = {
    type = "list",
    values = {"archived", "terminated", "started"}
  },

  Variants = {
    type = "set",
    constraints = {
      ["set_count"] = {
        {
          type = "count",
          value = {
            random = {
              lower = "2",
              upper = "8"
            }
          }
        }
      },
      percentage = {
        {
          type = "sum",
          value = "100",
        },
      },
      id = {
        {
          type = "unique"
        },
      },
      -- TODO indrajeet implement so many new constraint models
      name = {
        {
          type = "exactly_one",
          values = {"Control"}
        },
        {
          type = "unique"
        },
      },
      is_final = {
        {
          type = "exactly_one",
          values = {"1"}
        },
      },
      url = {
        {
          type = "exactly_one",
          values = {
            "http://localhost:8080/AB/test_webapp/ab_page_1.html",
            "http://localhost:8080/AB/test_webapp/ab_page_2.html",
            "http://localhost:8080/AB/test_webapp/ab_page_3.html",
            "http://localhost:8080/AB/test_webapp/ab_page_4.html",
            "http://localhost:8080/AB/test_webapp/ab_page_5.html",
            "http://localhost:8080/AB/test_webapp/ab_page_6.html",
            "http://localhost:8080/AB/test_webapp/ab_page_7.html",
            "http://localhost:8080/AB/test_webapp/ab_page_8.html"}
          },
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
        api_id = {
          -- TODO ask Ramesh
          type = "number",
          value = {
            random = {
              lower = "1",
              upper = "1",
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
        request_webapp_id = {
          -- TODO ask Ramesh
          type = "number",
          value = {
            random = {
              lower = "2",
              upper = "2",
            }
          },
        },
        description = {
          --TODO Ranesh double check
          type = "list",
          values = {"null"}
        },
        test_id = {
          -- TODO ask Ramesh
          type = "number",
          value = {
            random = {
              lower = "1",
              upper = "1",
            }
          },
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
        is_final = {
          type = "number",
          value = {
            random = {
              lower = "0",
              upper = "1",
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
        -- "custom_data": null,
        -- "created_at": "0000-00-00 00:00:00",
        -- "t_create": "0",
        -- "updated_at": "2018-02-05 15:35:39",
        -- "t_update": "151787373913040700"
      },
    },



  }


