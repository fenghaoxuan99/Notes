

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [__slots__ 限定类属性绑定](#__slots__-限定类属性绑定)
- [类之间的关系](#类之间的关系)
- [类继承](#类继承)
- [文件系统](#文件系统)
- [多进程](#多进程)
- [Python中的多线程](#python中的多线程)
  - [Python多线程的限制](#python多线程的限制)
  - [使用 `threading` 模块](#使用-threading-模块)
  - [示例代码](#示例代码)
  - [注意事项](#注意事项)
  - [应用场景](#应用场景)
- [锁（Lock）](#锁lock)
  - [RLock（可重入锁）](#rlock可重入锁)
  - [Semaphore（信号量）](#semaphore信号量)
  - [Condition（条件变量）](#condition条件变量)
  - [示例代码](#示例代码-1)
  - [注意事项](#注意事项-1)

<!-- /code_chunk_output -->


chr()：将整数转换成该编码对应的字符串（一个字符）。
ord()：将字符串（一个字符）转换成对应的编码（整数）。

在\后面还可以跟一个八进制或者十六进制数来表示字符，例如\141和\x61都代表小写字母a，前者是八进制的表示法，后者是十六进制的表示法。
如果不希望字符串中的\表示转义，我们可以通过在字符串的最前面加上字母r来加以说

Python并没有从语法上严格保证私有属性或方法的私密性，它只是给私有的属性和方法换了一个名字来妨碍对它们的访问，事实上如果你知道更换名字的规则仍然可以访问到它们:
```py
class Test:
    def __init__(self, foo):
        self.__foo = foo
    def __bar(self):
        print(self.__foo)
        print('__bar')
def main():
    test = Test('hello')
    test._Test__bar()
    print(test._Test__foo)
if __name__ == "__main__":
    main()
```

## __slots__ 限定类属性绑定
但是如果我们需要限定自定义类型的对象只能绑定某些属性，可以通过在类中定义__slots__变量来进行限定。需要注意的是__slots__的限定只对当前类的对象生效，对子类并不起任何作用。
__slots__ 只能用于限制实例属性，不能限制类属性。
一旦定义了 __slots__，就不能在实例中使用 __dict__ 或动态添加属性。
如果需要在子类中添加新的属性，可以通过定义 __dict__ 来实现，但这会丧失 __slots__ 的一些内存和效率优势。
```py
class Student(object):
    __slots__ = ('name', 'age') # 用tuple定义允许绑定的属性名称
    __slots__ = ['name', 'department', 'id']
```

## 类之间的关系
简单的说，类和类之间的关系有三种：is-a、has-a和use-a关系。

1. is-a关系也叫继承或泛化，比如学生和人的关系、手机和电子产品的关系都属于继承关系。
2. has-a关系通常称之为关联，比如部门和员工的关系，汽车和引擎的关系都属于关联关系；关联关系如果是整体和部分的关联，那么我们称之为聚合关系；如果整体进一步负责了部分的生命周期（整体和部分是不可分割的，同时同在也同时消亡），那么这种就是最强的关联关系，我们称之为合成关系。
3. use-a关系通常称之为依赖，比如司机有一个驾驶的行为（方法），其中（的参数）使用到了汽车，那么司机和汽车的关系就是依赖关系。


## 类继承

```py
class Parent(object):
      super().__init__(name, hp)
```

## 文件系统
![alt text](../Image/image.png)

```py
    try:
        f = open('致橡树.txt', 'r', encoding='utf-8')
        print(f.read())
    except FileNotFoundError:
        print('无法打开指定的文件!')
    except LookupError:
        print('指定了未知的编码!')
    except UnicodeDecodeError:
        print('读取文件时解码错误!')
    finally:
        if f:
            f.close()
```
finally块的代码不论程序正常还是异常都会执行到（甚至是调用了sys模块的exit函数退出Python环境，finally块都会被执行，因为exit函数实质上是引发了SystemExit异常），因此我们通常把finally块称为“总是执行代码块”，它最适合用来做释放外部资源的操作。如果不愿意在finally代码块中关闭文件对象释放资源，也可以使用上下文语法，通过with关键字指定文件对象的上下文环境并在离开上下文环境时自动释放文件资源，代码如下所示。



## 多进程
```py
from multiprocessing import Process
from os import getpid
from random import randint
from time import time, sleep
def download_task(filename):
    print('启动下载进程，进程号[%d].' % getpid())
    print('开始下载%s...' % filename)
    time_to_download = randint(5, 10)
    sleep(time_to_download)
    print('%s下载完成! 耗费了%d秒' % (filename, time_to_download))
def main():
    start = time()
    p1 = Process(target=download_task, args=('Python从入门到住院.pdf', ))
    p1.start()
    p2 = Process(target=download_task, args=('Peking Hot.avi', ))
    p2.start()
    p1.join()
    p2.join()
    end = time()
    print('总共耗费了%.2f秒.' % (end - start))
if __name__ == '__main__':
    main()
```
在上面的代码中，我们通过Process类创建了进程对象，通过target参数我们传入一个函数来表示进程启动后要执行的代码，后面的args是一个元组，它代表了传递给函数的参数。
Process对象的start方法用来启动进程，而join方法表示等待进程执行结束。运行上面的代码可以明显发现两个下载任务“同时”启动了，而且程序的执行时间将大大缩短，不再是两个任务的时间总和。

## 队列
在 Python 的 `multiprocessing` 模块中，`Queue` 类提供了一个进程安全的队列实现，允许你跨进程安全地发送和接收数据。
这对于进程间通信（IPC）非常有用。

```python
q = Queue()
```

### 将数据发送到队列

- **发送数据**：
  ```python
  q.put(item)
  ```

- **带时间限制发送数据**：
  ```python
  q.put(item, block=False, timeout=10)
  ```

### 从队列接收数据

- **接收数据**：
  ```python
  item = q.get()
  ```

- **带时间限制接收数据**：
  ```python
  item = q.get(block=False, timeout=10)
  ```

- **获取队列大小**：
  ```python
  size = q.qsize()
  ```

### 示例代码
```python
from multiprocessing import Process, Queue

def producer(queue):
    for i in range(5):
        print(f"生产了: {i}")
        queue.put(i)
    queue.put(None)  # 发送结束信号

def consumer(queue):
    while True:
        item = queue.get()
        if item is None:
            break
        print(f"消费了: {item}")

if __name__ == "__main__":
    q = Queue()

    producer_process = Process(target=producer, args=(q,))
    consumer_process = Process(target=consumer, args=(q,))

    producer_process.start()
    consumer_process.start()

    producer_process.join()
    consumer_process.join()
```

### 注意事项

1. **进程安全**：`multiprocessing.Queue` 是进程安全的，可以在不同进程之间安全地传递数据。
2. **阻塞操作**：`get()` 和 `put()` 方法默认是阻塞的，直到队列中有项目可用或可以放入项目。
3. **结束信号**：通常使用 `None` 或其他特定的值作为结束信号，通知消费者进程停止接收数据。
4. **队列大小**：可以通过 `qsize()` 方法获取队列中的项目数量，但这个方法在多进程环境中可能不完全准确。
5. **队列关闭**：在所有数据传输完成后，可以使用 `queue.close()` 方法关闭队列，并使用 `queue.join_thread()` 等待其他线程完成操作。

### 应用场景

- **进程间通信**：在多进程应用程序中，使用 `multiprocessing.Queue` 进行进程间的数据传输。
- **数据生产者和消费者**：实现生产者-消费者模式，其中一个或多个进程生成数据，另一个或多个进程消费数据。

通过使用 `multiprocessing.Queue`，你可以在多进程环境中安全、高效地进行数据传输和任务调度。

## Python中的多线程
Python中的多线程是指在同一个进程中并行运行多个线程。线程是操作系统能够进行运算调度的最小单位，它被包含在进程之中，是进程中的实际运作单位。

### Python多线程的限制
Python的多线程主要受限于全局解释器锁（Global Interpreter Lock，GIL）。GIL确保在任何时刻只有一个线程在执行Python字节码。这意味着即使在多核处理器上，Python的线程也可能无法并行执行。因此，Python的多线程主要用于I/O密集型任务，而对于CPU密集型任务，多线程带来的性能提升有限。

### 使用 `threading` 模块
Python的`threading`模块是实现多线程的主要方式。以下是基本用法：

1. **导入模块**：
   ```python
   import threading
   ```

2. **定义目标函数**：
   目标函数是线程执行的代码。
   ```python
   def worker():
       print(f"线程{threading.current_thread().name}正在执行")
   ```

3. **创建线程对象**：
   使用`threading.Thread`类创建线程对象，并指定目标函数。
   ```python
   t = threading.Thread(target=worker)
   ```

4. **启动线程**：
   调用线程对象的`start()`方法启动线程。
   ```python
   t.start()
   ```

5. **等待线程结束**：
   调用线程对象的`join()`方法等待线程结束。
   ```python
   t.join()
   ```

### 示例代码

```python
import threading
import time

def worker(name):
    print(f"线程{name}开始执行")
    time.sleep(1)
    print(f"线程{name}结束执行")

if __name__ == "__main__":
    threads = []
    for i in range(5):
        t = threading.Thread(target=worker, args=(i,))
        t.start()
        threads.append(t)
    
    for t in threads:
        t.join()

    print("所有线程已结束")
```

### 注意事项

- **全局解释器锁（GIL）**：由于GIL的存在，Python的线程在执行CPU密集型任务时可能不会带来预期的性能提升。
- **线程安全**：  在多线程环境中，共享资源的访问需要特别注意线程安全问题，避免数据竞争和死锁。
- **线程间通信**：可以使用`threading`模块中的`Lock`、`Event`、`Condition`、`Semaphore`等工具实现线程间的通信和同步。

### 应用场景

- **I/O密集型任务**：如文件读写、网络请求等，多线程可以提高程序的响应性和效率。
- **并行执行多个任务**：需要同时执行多个独立任务时，可以使用多线程实现并行处理。

尽管Python的多线程在CPU密集型任务中受限于GIL，但在I/O密集型任务和需要提高响应性的场景中，多线程仍然是一个有效的工具。



## 锁（Lock）

在多线程编程中，锁（Locks）是一种基本的同步机制，用于控制多个线程对共享资源的访问。当一个线程需要操作共享资源时，它可以先尝试获取锁；如果成功，线程可以安全地访问资源；操作完成后，线程释放锁，允许其他线程访问资源。

- **创建锁**：
  ```python
  lock = threading.Lock()
  ```

- **获取锁**：
  ```python
  lock.acquire()
  ```

- **释放锁**：
  ```python
  lock.release()
  ```

### RLock（可重入锁）

允许同一个线程多次获取锁。

- **创建可重入锁**：
  ```python
  rlock = threading.RLock()
  ```

- **获取可重入锁**：
  ```python
  rlock.acquire()
  ```

- **释放可重入锁**：
  ```python
  rlock.release()
  ```

### Semaphore（信号量）

控制对一定数量资源的访问。

- **创建信号量**：
  ```python
  semaphore = threading.Semaphore(2)
  ```

- **获取信号量**：
  ```python
  semaphore.acquire()
  ```

- **释放信号量**：
  ```python
  semaphore.release()
  ```

### Condition（条件变量）

允许一个或多个线程在某个条件满足之前挂起，并在条件满足时被唤醒。

- **创建条件变量**：
  ```python
  condition = threading.Condition()
  ```

- **等待条件**：
  ```python
  condition.wait()
  ```

- **通知线程**：
  ```python
  condition.notify()
  ```

- **通知所有线程**：
  ```python
  condition.notify_all()
  ```

### 示例代码

以下是一个使用锁的示例，演示了如何在多线程环境中同步对共享资源的访问：
```python
import threading

# 共享资源
counter = 0

# 锁
lock = threading.Lock()

def increment():
    global counter
    for _ in range(100000):
        lock.acquire()  # 获取锁
        try:
            counter += 1
        finally:
            lock.release()  # 释放锁

if __name__ == "__main__":
    threads = []
    for _ in range(10):
        t = threading.Thread(target=increment)
        t.start()
        threads.append(t)

    for t in threads:
        t.join()

    print(f"Final counter value: {counter}")
```
在这个示例中：
- 定义了一个共享资源 `counter`。
- 使用 `threading.Lock()` 创建了一个锁。
- 在 `increment` 函数中，每次增加 `counter` 之前先获取锁，操作完成后释放锁，确保在多线程环境中对 `counter` 的访问是安全的。

### 注意事项
- **死锁**：如果不正确地管理锁，可能会导致死锁，即两个或多个线程相互等待对方释放锁。
- **上下文管理**：可以使用 `with` 语句自动管理锁的获取和释放，例如 `with lock:`。
- **公平性**：锁的获取应该尽可能公平，避免某些线程长时间无法获取锁。


## 计算密集型 vs. IO密集型
是否采用多任务的第二个考虑是任务的类型。我们可以把任务分为计算密集型和IO密集型。

计算密集型任务的特点是要进行大量的计算，消耗CPU资源，比如计算圆周率、对视频进行高清解码等等，全靠CPU的运算能力。这种计算密集型任务虽然也可以用多任务完成，但是任务越多，花在任务切换的时间就越多，CPU执行任务的效率就越低，所以，要最高效地利用CPU，计算密集型任务同时进行的数量应当等于CPU的核心数。

计算密集型任务由于主要消耗CPU资源，因此，代码运行效率至关重要。Python这样的脚本语言运行效率很低，完全不适合计算密集型任务。对于计算密集型任务，最好用C语言编写。

第二种任务的类型是IO密集型，涉及到网络、磁盘IO的任务都是IO密集型任务，这类任务的特点是CPU消耗很少，任务的大部分时间都在等待IO操作完成（因为IO的速度远远低于CPU和内存的速度）。对于IO密集型任务，任务越多，CPU效率越高，但也有一个限度。常见的大部分任务都是IO密集型任务，比如Web应用。

IO密集型任务执行期间，99%的时间都花在IO上，花在CPU上的时间很少，因此，用运行速度极快的C语言替换用Python这样运行速度极低的脚本语言，完全无法提升运行效率。对于IO密集型任务，最合适的语言就是开发效率最高（代码量最少）的语言，脚本语言是首选，C语言最差。