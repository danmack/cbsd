--- vmm.c.bak	2018-03-16 17:40:37.960425000 +0300
+++ vmm.c	2018-03-16 17:59:26.133945000 +0300
@@ -165,6 +165,11 @@
 	struct vmspace	*vmspace;		/* (o) guest's address space */
 	char		name[VM_MAX_NAMELEN];	/* (o) virtual machine name */
 	struct vcpu	vcpu[VM_MAXCPU];	/* (i) guest vcpus */
+	/* The following describe the vm cpu topology */
+	uint16_t        sockets;                /* (o) num of sockets */
+	uint16_t        cores;                  /* (o) num of cores/socket */
+	uint16_t        threads;                /* (o) num of threads/core */
+	uint16_t        maxcpus;                /* (o) max pluggable cpus */
 };
 
 static int vmm_initialized;
@@ -423,6 +428,9 @@
 		vcpu_init(vm, i, create);
 }
 
+extern u_int cores_per_package;
+extern u_int threads_per_core;
+
 int
 vm_create(const char *name, struct vm **retvm)
 {
@@ -448,10 +456,41 @@
 	vm->vmspace = vmspace;
 	mtx_init(&vm->rendezvous_mtx, "vm rendezvous lock", 0, MTX_DEF);
 
+	vm->sockets = 1;
+	vm->cores = cores_per_package;  /* XXX backwards compatibility */
+	vm->threads = threads_per_core; /* XXX backwards compatibility */
+	vm->maxcpus = 0;                /* XXX not implemented */
+
 	vm_init(vm, true);
 
 	*retvm = vm;
 	return (0);
+}
+
+void
+vm_get_topology(struct vm *vm, uint16_t *sockets, uint16_t *cores,
+    uint16_t *threads, uint16_t *maxcpus)
+{
+	*sockets = vm->sockets;
+	*cores = vm->cores;
+	*threads = vm->threads;
+	*maxcpus = vm->maxcpus;
+}
+
+int
+vm_set_topology(struct vm *vm, uint16_t sockets, uint16_t cores,
+    uint16_t threads, uint16_t maxcpus)
+{
+	if (maxcpus != 0)
+		return (EINVAL);        /* XXX remove when supported */
+	if ((sockets * cores * threads) > VM_MAXCPU)
+		return (EINVAL);
+	/* XXX need to check sockets * cores * threads == vCPU, how? */
+	vm->sockets = sockets;
+	vm->cores = cores;
+	vm->threads = threads;
+	vm->maxcpus = maxcpus;
+	return(0);
 }
 
 static void
