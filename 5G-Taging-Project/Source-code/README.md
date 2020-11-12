## 5G Paging Simulation 

___ 

#### System Model
\- We propose a client-server model with AMF, UEs as clients (TCP client) and gNodeB as a server (TCP server) by estabishing TCP socket.

![PyzCH.png](https://s1.imghub.io/PyzCH.png)

#### How it works ? 

\- Firstly, AMF declares a struct of packet including 4 fields (**Message Type**, **UE ID**, **TAC** and **CN_Domain**) and then send it to gNodeB.

\- Then, gNodeB pushes received UE IDs to a queue and checks 3 fields **Message Type**, **TAC** and **CN_Domain** whether the values of those equal 100. If not equal, gNodeB will ignore it. Else, it will wait **WakeUp** packet that its value equal **True** from UE and then sent correspondingly UE IDs to that UEs from queue.

#### Usage 

> Both **Client.c** and **Server.c** files should be ran on **CodeBlock**.
