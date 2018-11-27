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
    function(json_body)
        local classify = require 'lupa/classify'
        return classify(json_body)
    end
    """

release_str = \
    """
    function()
        local release = require 'lupa/release'
        return release()
    end
    """


def init(model_dir, forest_type=None):
    func = executor.eval(init_str)
    result = func(model_dir, forest_type)
    return result


def classify(json_body):
    func = executor.eval(classify_str)
    result = func(json_body)
    return result


def release():
    func = executor.eval(release_str)
    result = func()
    return result
