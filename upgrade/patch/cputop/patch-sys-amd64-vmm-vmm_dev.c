--- vmm_dev.c.bak	2018-03-16 17:40:37.959597000 +0300
+++ vmm_dev.c	2018-03-16 18:04:58.840259000 +0300
@@ -346,6 +346,7 @@
 	struct vm_rtc_time *rtctime;
 	struct vm_rtc_data *rtcdata;
 	struct vm_memmap *mm;
+	struct vm_cpu_topology *topology;
 	uint64_t *regvals;
 	int *regnums;
 
@@ -726,6 +727,17 @@
 		break;
 	case VM_RESTART_INSTRUCTION:
 		error = vm_restart_instruction(sc->vm, vcpu);
+		break;
+	case VM_SET_TOPOLOGY:
+		topology = (struct vm_cpu_topology *)data;
+		error = vm_set_topology(sc->vm, topology->sockets,
+			topology->cores, topology->threads, topology->maxcpus);
+		break;
+	case VM_GET_TOPOLOGY:
+		topology = (struct vm_cpu_topology *)data;
+		vm_get_topology(sc->vm, &topology->sockets, &topology->cores,
+			&topology->threads, &topology->maxcpus);
+		error = 0;
 		break;
 	default:
 		error = ENOTTY;
