MPP Design Document
1.  Overview

    The Modular Processing Platform (MPP) is a microservices-based system that allows users to define, execute, and extend commands dynamically. It is designed with super modularity, super extensibility, and super scalability in mind.

    The MPP runs with UNIX sockets.  The idea behind this decision is that it allows the MPP user to leverage the services offered by the OS including multithreading, memory mangement, file handling, ethernet interface handling, sockets and tty/pty.  Users of MPP have lots of freedom and power.  All commands in the MPP ecosystem are crafted in open source code, either c/c++ or bash scripts.  Any node of an MPP system can communicate with another node through IPC and unix sockets, or TCP.  Nodes may perform ftp operations.  Commands are executed anonymously.  MPP implements a Domain Specific Language offering very rich possibilties for an MPP user.

2.  Definition Checklist

    Every new component must be fully defined before implementation.  Each component must have:

    - UML class diagram
    - Interface definition clearly defining APIs or contracts
    - Usage scenarios complete with UML sequence diagrams
    - Tests defined before implementation
    - Configuration consideratons addressed
    - Failure handling plan, how a component handles errors


3.  Command Processing System (CPS)

    The CPS is unique compared to most software applications.  MPP only has 7 built-in commands: quit, q, exit, EXC, help, h, ?.  Every other command is in the form of a c++ program.  Commands are copiled to a .so format and loaded and ran by MPP.  Other user input to the MPP can be in the form of MPP's domain specific language (DSL).  The DSL employs concepts such as a software grammar, parsing techniques, functional programming, handling input and output between commands, handling state, functional programming, pipes, scripting and more.

    The CPS is oblivious to the identity and type of commands it handles.  

    - Receiving commands through UNIX sockets.
    Parsing and validating commands.
    Executing the appropriate registered command handler.

4.  Domain Specific Language (DSL)

    MPP has a concept of its Ethos and Character (EandC).  It's a collection of things that go hand-in-hand to acheive an overall productive and free work environment for serious computer users.  The MPP EandC always defers to freedom and always defers consequences and responsibilities to the MPP user.  MPP provides complete freedom.  MPP will never impose a restriction on an MPP user in anyway.  For the purpose of creating microservices the MPP will provide some services like memory management (mpp_maloc()), plug-in capability, context loading.
    
    - Each component has a ./commands default directory which will be checked first by MPP. 
    - MPP has some linux-like features such as command path configuration and file organization and scripting capabilities.
    - The DSL will be a formally defined software language that's small and will lend itself well to user's who enjoy powerful command line capabilites and who will consider programs made with the DSL to be precious.
    - ftp will be common for an MPP user as nodes can communicate through IPC or TCP.  MPP can be an extensive network or just a single node.

    Is there a need for a core, partner?  Could core be optional, for users who want to extend with their own microservices that need an MPP service such as mpp_maloc().  Otherwise components can couple directly and use capabilities (CAPS) that are offered by various components.  Tasks may be delegated to remote components on different machines (IP addresses).

5.  Development Process

    MPP arranged as:
    - VSCode project /usr/local/unix
    - Makefile build system
    - Unit tests run through Makefile
    - Integration tests are script-based currently
    - Using TDD framework catch2
    - git repo https://github.com/ambersoj/MPP

    See new component 2. Definition Checklist above.
    The git pull request system is utiilized.  Git tracks code as well a the written and graphic documetation such as the MDD and UML diagrams.

