import libcxx.test.executor


class NoopExecutor(libcxx.test.executor.Executor):
    def __init__(self, target_info):
        super(NoopExecutor, self).__init__()
        self.target_info = target_info

    def run(self, exe_path, cmd=None, work_dir='.', file_deps=None, env=None):
        cmd = cmd or [exe_path]
        return (cmd, '', '', 0)
