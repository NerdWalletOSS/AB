import lupa


lua = lupa.LuaRuntime(unpack_returned_tuples=True)


class Executor:
    def __init__(self, debug=False):
        self.debug = debug

    def eval(self, code):
        try:
            result = lua.eval(code)
        except Exception as exc:
            result = str(exc)
        if self.debug:
            with open("/tmp/output.txt", "a") as f:
                f.write("%s\n" % str(result))
        return result

    def execute(self, code):
        try:
            result = lua.execute(code)
        except Exception as exc:
            result = str(exc)
        if self.debug:
            with open("/tmp/output.txt", "a") as f:
                f.write("%s\n" % str(result))
        return result

