
## subprocess 
推荐使用 subprocess 模块中的函数，如 subprocess.run()，来执行外部命令。subprocess.run() 提供了更多的控制和灵活性，同时更加安全。
**语法格式：**
```py{.line-numbers}
subprocess.run(args, *, stdin=None, input=None, stdout=None, stderr=None, shell=False, timeout=None, check=False, universal_newlines=False)

subprocess.call(args, *, stdin=None, stdout=None, stderr=None, shell=False, timeout=None)

subprocess.check_call(args, *, stdin=None, stdout=None, stderr=None, shell=False, timeout=None)

subprocess.check_output(args, *, stdin=None, stderr=None, shell=False, universal_newlines=False, timeout=None)

subprocess.getstatusoutput(cmd)

subprocess.getoutput(cmd)
```

#### 参数说明：

- args： 要执行的shell命令，默认应该是一个字符串序列，如[‘df’, ‘-Th’]或(‘df’, ‘-Th’)，也可以是一个字符串，如’df -Th’，但是此时需要把shell参数的值置为True。
- shell： 如果shell为True，那么指定的命令将通过shell执行。如果我们需要访问某些shell的特性，如管道、文件名通配符、环境变量扩展功能，这将是非常有用的。当然，python本身也提供了许多类似shell的特性的实现，如glob、fnmatch、os.walk()、os.path.expandvars()、os.expanduser()和shutil等。
- check： 如果check参数的值是True，且执行命令的进程以非0状态码退出，则会抛出一个CalledProcessError的异常，且该异常对象会包含 参数、退出状态码、以及stdout和stderr(如果它们有被捕获的话)。
- stdout, stderr：input： 该参数是传递给Popen.communicate()，通常该参数的值必须是一个字节序列，如果universal_newlines=True，则其值应该是一个字符串。
run()函数默认不会捕获命令执行结果的正常输出和错误输出，如果我们向获取这些内容需要传递subprocess.PIPE，然后可以通过返回的CompletedProcess类实例的stdout和stderr属性或捕获相应的内容；
call()和check_call()函数返回的是命令执行的状态码，而不是CompletedProcess类实例，所以对于它们而言，stdout和stderr不适合赋值为subprocess.PIPE；
check_output()函数默认就会返回命令执行结果，所以不用设置stdout的值，如果我们希望在结果中捕获错误信息，可以执行stderr=subprocess.STDOUT。
- universal_newlines： 该参数影响的是输入与输出的数据格式，比如它的值默认为False，此时stdout和stderr的输出是字节序列；当该参数的值设置为True时，stdout和stderr的输出是字符串。

**实例：**
```py{.line-numbers}
import subprocess
# 使用 subprocess.run() 替代 os.system()
result = subprocess.run(['ls', '-l'], capture_output=True, text=True)
print('Return code:', result.returncode)
print('Output:\n',    result.stdout)
```

- args：命令和参数列表。
- capture_output：布尔值，如果为 True，则 stdout 和 stderr 将被捕获。
- text： 布尔值，如果为 True，则 stdout 和 stderr 将作为文本返回，而不是字节。
- shell：布尔值，如果为 True，则会通过 shell 执行命令。  

在这个示例中，subprocess.run() 执行了 ls -l 命令，并捕获了输出。capture_output=True 表示捕获标准输出和标准错误输出，
text=True 表示将输出作为文本处理。这种方式比 os.system() 更加安全和灵活。

subprocess.run()函数返回值是一个**subprocess.CompletedPorcess类的实例,它表示的是一个已结束进程的状态信息**，它所包含的属性如下：

- args： 用于加载该进程的参数，这可能是一个列表或一个字符串
- returncode： 子进程的退出状态码。通常情况下，退出状态码为0则表示进程成功运行了；一个负值-N表示这个子进程被信号N终止了
- stdout： 从子进程捕获的stdout。这通常是一个字节序列，如果run()函数被调用时指定universal_newlines=True，则该属性值是一个字符串。如果run()函数被调用时指定stderr=subprocess.STDOUT，那么stdout和stderr将会被整合到这一个属性中，且stderr将会为None
- stderr： 从子进程捕获的stderr。它的值与stdout一样，是一个字节序列或一个字符串。如果stderr灭有被捕获的话，它的值就为None
- check_returncode()： 如果returncode是一个非0值，则该方法会抛出一个CalledProcessError异常。


## 标准输出（stdout）：
每个进程都有一个标准输出流，这是进程用于向其父进程发送信息的通道。默认情况下，当一个 Python 脚本启动一个新进程时，该进程的标准输出会打印到控制台。

## subprocess.PIPE：
subprocess.PIPE 是一个特殊的值，用来告诉 subprocess.Popen() 创建一个管道（pipe），而不是将子进程的标准输出重定向到控制台。这样，你就可以在 Python 脚本中捕获子进程的输出，而不必让它直接打印到控制台。

示例用法：
```py
import subprocess
# 创建一个子进程，并将标准输出重定向到一个管道
process = subprocess.Popen(['ls', '-l'], stdout=subprocess.PIPE, text=True)
# 从管道中读取输出
output = process.stdout.read()
# 打印输出内容
print(output)
```
在这个例子中，['ls', '-l'] 是要执行的命令和参数列表。通过设置 stdout=subprocess.PIPE，我们创建了一个管道，子进程的输出不会直接打印到控制台，而是可以被 Python 脚本捕获和处理。

#### 为什么使用 PIPE？
捕获输出：允许你捕获子进程的输出，这对于日志记录、错误处理或进一步处理输出数据非常有用。
避免阻塞：如果你不捕获输出，并且子进程产生大量输出，那么子进程可能会被其输出缓冲区阻塞。
控制输出：你可以控制输出的去向，例如，可以将其写入文件、发送到网络或进行其他形式的处理。
使用 subprocess.PIPE 可以提供对子进程输出的精细控制，是 subprocess 模块功能强大的原因之一。

subprocess.run()
subprocess.run() 是 Python 3.5 引入的一个高级接口，用于执行子进程并等待其完成。
它是一个函数，接受命令和参数作为输入，并返回一个**CompletedProcess**实例，其中包含执行结果。
它通常用于简单的用例，当你需要快速执行一个命令并获取其输出或状态码时。
subprocess.run() 会自动处理输入、输出和错误管道，使得使用起来更加方便和安全。
**它不能用于长时间运行的子进程，因为它会等待子进程结束。**

subprocess.Popen()
subprocess.Popen() 是一个低级接口，用于创建新的进程，提供对进程的更多控制。
它是一个类，你需要实例化它来创建一个进程对象。
它允许你设置更复杂的参数，如环境变量、工作目录、输入/输出/错误管道等。
**它适用于需要长时间运行或需要与子进程进行交互的场景。**
你需要手动管理与子进程的通信，如读取输出、发送输入、等待进程结束等。  

#### 主要区别：
- 使用场景：subprocess.run() 适合简单的一次性命令执行，而 subprocess.Popen() 适合需要更多控制的场景。
- 返回值：subprocess.run() 返回一个 CompletedProcess 对象，而 subprocess.Popen() 返回一个 Popen 对象。
- 管道管理：subprocess.run() 自动处理管道，而 subprocess.Popen() 需要你手动管理管道。
- 等待子进程：subprocess.run() 会等待子进程结束，而 subprocess.Popen() 可以通过调用 wait() 方法来等待子进程结束。
- 交互性：subprocess.Popen() 支持与子进程进行交互，如发送输入、读取输出等，而 subprocess.run() 不支持。
总的来说，subprocess.run() 更简单易用，适合大多数基本用例，而 subprocess.Popen() 更灵活，适合需要高级控制的场景。