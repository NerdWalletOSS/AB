import json
from lupa_bridge import Executor


executor = Executor()
init_str = \
    """
    function(model_dir, forest_type)
        local init = require 'lupa/init'
        return init(model_dir, forest_type)
    end
    """

classify_str = \
    """
    function(g_inerp, json_body)
        local classify = require 'lupa/classify'
        return classify(g_inerp, json_body)
    end
    """

release_str = \
    """
    function(g_interp)
        local release = require 'lupa/release'
        return release(g_interp)
    end
    """


def init(model_dir, forest_type=None):
    func = executor.eval(init_str)
    result = func(model_dir, forest_type)
    return result


def classify(g_inerp, json_body):
    func = executor.eval(classify_str)
    result = func(g_inerp, json_body)
    return result


def release(g_inerp):
    func = executor.eval(release_str)
    result = func(g_inerp)
    return result
