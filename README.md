1.Каким образом реализована идея “Everything is Object”, в чем этом выражается?
Приложениям пользовательского уровня может управлять объектами по средствам вызовов, объекты являются нити и сервисы, 
которыми занимается ядро ОС.

2.Что такое Capability list, зачем он нужен?
Приложение-сервер делает вызов(invoke, call) из Capability Table по номеру. Из соображений безопасности пользователю не 
доступна напрямую запись на диск по файловому дескриптору, этим занимается ядро операционной системы.
Исходный набор способностей - это родительский процесс, механизм выделения памяти, главная нить, лог-файлы и так далее.

3.Где находится scheduler и почему?
(ps -aux)
Планировщик - это объект ядра.
Реализует политики ядра и передачу нитей CPU, включая CPU статистику.

4.Что такое Sigma0, Moe, Ned, и зачем они нужны?
Ned - init-подобная система загрузки(загружать драйвера и приложения*?).
Sigma и Moe -  root-pager и root-task соответственно.
(paging - механизм распределения памяти, pager устройства для работу с ней) 
У sigma0 изначально находятся все системные ресурсы, но обычно он используется для разрешения page faults от 
заданий суперпользователя из Moe. Moe предоставляет главные ресурсы нормальным пользовательским приложениям таким как 
первоначальный загрузчик задач, и сервис размещения памяти для управления виртуальной памятью и выделению памяти(в data space).

5.Что собой представляет L4Re?

L4 low level kernel runtime environment.
Представляет базовый набор сервисов и абстракций, которые полезны для реализации и запуска приложений пользовательского
уровня поверх микроядра Fiasko.OC
В состав L4Re входит набор библиотек и серверов. Библиотеки, как и интерфейсы сервера полностью обектно-ориентированные.
Они образуют прототип реализаций для, определенный спецификацией L4Re.
Минимальная его реализация требует, чтобы 3 компонента были загружены заранее: микроядро Fiasko.OC, root-pager и root-task



6.Что является execution unit в концепции Fiasco.OC ?

7.Что такое TCB, чет отличается от UTCB, что такое Kernel Info Block, где находится каждая из этих структур, зачем она нужна и как с ними можно работать?
TCB - блок управления нитями, делится на два уровня
KTCB - блок управления нитями ядра, которые можно модифицировать с помощью syscalls
UTCB - блок управления нитями пользовательского уровня, те нити которые можно предоставить пользователю для чтения/записи
без ущерба безопансости. Они включают виртуальные регистры(так как они не связаны с реальными реальными регистрами).
Могут модифицироваться только через библиотечные вызовы(в противном случае не может быть гарантирована их консистентность
(коллизии на чтение/запись).
Многие из полей UTCB можно модифицировать только как сторонние эффекты некоторых операций с IPC.

8.Какие механизмы сериализации доступа к переменным были озвучены в лекции?
вызов L4_IPC_never для блокировки вызовов (invokes).
Использование mutex'ов.
Thread switch?

9.Что делает и зачем нужны l4_msgtag и l4_ipc_send?
Messages(сообщения) - это механизм взаимосвязи между объектами.
Чтобы их передавать необходимо установить протокол передачи сообщений и количество слов передаваемых по UTCB,
для этого и существует l4_msgtag.
l4_ipc_send - механизм передачи сообщений.



