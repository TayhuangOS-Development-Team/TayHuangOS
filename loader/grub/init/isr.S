.extern PrimaryIRQHandler
.extern PrimaryExceptionHandler

.macro menter
    pushl %esp
    pushl %ebp

    movl %cr3, %ebp
    pushl %ebp

    movw %ds, %bp
    pushl %ebp
    movw %es, %bp
    pushl %ebp
    movw %fs, %bp
    pushl %ebp
    movw %gs, %bp
    pushl %ebp

    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esi
    pushl %edi
.endm

.macro mleave
    popl %edi
    popl %esi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax

    popl %ebp
    movw %bp, %gs
    popl %ebp
    movw %bp, %fs
    popl %ebp
    movw %bp, %ds
    popl %ebp
    movw %bp, %ds

    popl %ebp
    movl %ebp, %cr3

    popl %ebp
    popl %esp
.endm

.extern errcode
.type errcode, @object

.macro NonErrcodeHandler vector
    movl $0xFFFFFFFF, (errcode)
    menter

    pushl %esp
    movl $\vector, %eax

    call PrimaryExceptionHandler

    addl $4, %esp

    mleave
    iret
.endm

.macro ErrcodeHandler vector
    pushl %eax
    movl 4(%esp), %eax
    movl %eax, (errcode)
    popl %eax
    add $4, %esp
    menter

    pushl %esp
    movl $\vector, %eax

    call PrimaryExceptionHandler

    addl $4, %esp

    mleave
    iret
.endm

.global DivideByZeroFaultHandler
.type   DivideByZeroFaultHandler, @function
.global SingleStepTrapHandler
.type   SingleStepTrapHandler, @function
.global NMIHandler
.type   NMIHandler, @function
.global BreakpointTrapHandler
.type   BreakpointTrapHandler, @function
.global OverflowTrapHandler
.type   OverflowTrapHandler, @function
.global BoundRangeExceededFaultHandler
.type   BoundRangeExceededFaultHandler, @function
.global InvalidOpcodeFaultHandler
.type   InvalidOpcodeFaultHandler, @function
.global DeviceNotAvailableFaultHandler
.type   DeviceNotAvailableFaultHandler, @function
.global DoubleFaultHandler
.type   DoubleFaultHandler, @function
.global CoprocessorSegmentOverrunFaultHandler
.type   CoprocessorSegmentOverrunFaultHandler, @function
.global InvalidTSSFault
.type   InvalidTSSFault, @function
.global SegmentNotPresentFaultHandler
.type   SegmentNotPresentFaultHandler, @function
.global StackSegmentFaultHandler
.type   StackSegmentFaultHandler, @function
.global GeneralProtectionFaultHandler
.type   GeneralProtectionFaultHandler, @function
.global PageFaultHandler
.type   PageFaultHandler, @function
.global ReservedHandler1
.type   ReservedHandler1, @function
.global X87FloatingPointFaultHandler
.type   X87FloatingPointFaultHandler, @function
.global AlignmentCheckHandler
.type   AlignmentCheckHandler, @function
.global MachineCheckHandler
.type   MachineCheckHandler, @function
.global SIMDFloatingPointFaultHandler
.type   SIMDFloatingPointFaultHandler, @function
.global VirtualizationFaultHandler
.type   VirtualizationFaultHandler, @function
.global ControlProtectionFaultHandler
.type   ControlProtectionFaultHandler, @function
.global ReservedHandler2
.type   ReservedHandler2, @function
.global ReservedHandler3
.type   ReservedHandler3, @function
.global ReservedHandler4
.type   ReservedHandler4, @function
.global ReservedHandler5
.type   ReservedHandler5, @function
.global ReservedHandler6
.type   ReservedHandler6, @function
.global ReservedHandler7
.type   ReservedHandler7, @function
.global HypervisorInjectionException
.type   HypervisorInjectionException, @function
.global VMMCommunicationFaultHandler
.type   VMMCommunicationFaultHandler, @function
.global SecurityFaultHandler
.type   SecurityFaultHandler, @function
.global ReservedHandler8
.type   ReservedHandler8, @function

DivideByZeroFaultHandler:
    NonErrcodeHandler 0
SingleStepTrapHandler:
    NonErrcodeHandler 1
NMIHandler:
    NonErrcodeHandler 2
BreakpointTrapHandler:
    NonErrcodeHandler 3
OverflowTrapHandler:
    NonErrcodeHandler 4
BoundRangeExceededFaultHandler:
    NonErrcodeHandler 5
InvalidOpcodeFaultHandler:
    NonErrcodeHandler 6
DeviceNotAvailableFaultHandler:
    NonErrcodeHandler 7
DoubleFaultHandler:
    ErrcodeHandler 8
CoprocessorSegmentOverrunFaultHandler:
    NonErrcodeHandler 9
InvalidTSSFault:
    ErrcodeHandler 10
SegmentNotPresentFaultHandler:
    ErrcodeHandler 11
StackSegmentFaultHandler:
    ErrcodeHandler 12
GeneralProtectionFaultHandler:
    ErrcodeHandler 13
PageFaultHandler:
    ErrcodeHandler 14
ReservedHandler1:
    NonErrcodeHandler 15
X87FloatingPointFaultHandler:
    NonErrcodeHandler 16
AlignmentCheckHandler:
    ErrcodeHandler 17
MachineCheckHandler:
    NonErrcodeHandler 18
SIMDFloatingPointFaultHandler:
    NonErrcodeHandler 19
VirtualizationFaultHandler:
    NonErrcodeHandler 20
ControlProtectionFaultHandler:
    ErrcodeHandler 21
ReservedHandler2:
    NonErrcodeHandler 22
ReservedHandler3:
    NonErrcodeHandler 23
ReservedHandler4:
    NonErrcodeHandler 24
ReservedHandler5:
    NonErrcodeHandler 25
ReservedHandler6:
    NonErrcodeHandler 26
ReservedHandler7:
    NonErrcodeHandler 27
HypervisorInjectionException:
    NonErrcodeHandler 28
VMMCommunicationFaultHandler:
    ErrcodeHandler 29
SecurityFaultHandler:
    ErrcodeHandler 30
ReservedHandler8:
    NonErrcodeHandler 31

//-------------------------------------------------

.macro irqHandler vector
    menter

    pushl %esp
    movl $\vector, %eax

    call PrimaryIRQHandler

    addl $4, %esp

    mleave
    iret
.endm

.global Irq0Handler
.type   Irq0Handler, @function
.global Irq1Handler
.type   Irq1Handler, @function
.global Irq2Handler
.type   Irq2Handler, @function
.global Irq3Handler
.type   Irq3Handler, @function
.global Irq4Handler
.type   Irq4Handler, @function
.global Irq5Handler
.type   Irq5Handler, @function
.global Irq6Handler
.type   Irq6Handler, @function
.global Irq7Handler
.type   Irq7Handler, @function
.global Irq8Handler
.type   Irq8Handler, @function
.global Irq9Handler
.type   Irq9Handler, @function
.global Irq10Handler
.type   Irq10Handler, @function
.global Irq11Handler
.type   Irq11Handler, @function
.global Irq12Handler
.type   Irq12Handler, @function
.global Irq13Handler
.type   Irq13Handler, @function
.global Irq14Handler
.type   Irq14Handler, @function
.global Irq15Handler
.type   Irq15Handler, @function

Irq0Handler:
    irqHandler 0
Irq1Handler:
    irqHandler 1
Irq2Handler:
    irqHandler 2
Irq3Handler:
    irqHandler 3
Irq4Handler:
    irqHandler 4
Irq5Handler:
    irqHandler 5
Irq6Handler:
    irqHandler 6
Irq7Handler:
    irqHandler 7
Irq8Handler:
    irqHandler 8
Irq9Handler:
    irqHandler 9
Irq10Handler:
    irqHandler 10
Irq11Handler:
    irqHandler 11
Irq12Handler:
    irqHandler 12
Irq13Handler:
    irqHandler 13
Irq14Handler:
    irqHandler 14
Irq15Handler:
    irqHandler 15
