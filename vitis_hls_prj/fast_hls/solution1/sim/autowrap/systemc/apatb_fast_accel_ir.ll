; ModuleID = 'D:/VitisHLSProjects/fast_hls/solution1/.autopilot/db/a.g.ld.5.gdce.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-i64:64-i128:128-i256:256-i512:512-i1024:1024-i2048:2048-i4096:4096-n8:16:32:64-S128-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024"
target triple = "fpga64-xilinx-none"

%struct.ap_uint = type { %struct.ap_int_base }
%struct.ap_int_base = type { %struct.ssdm_int }
%struct.ssdm_int = type { i8 }

; Function Attrs: noinline
define void @apatb_fast_accel_ir(%struct.ap_uint* %img_in, i32 %threshold, %struct.ap_uint* %img_out, i32 %rows, i32 %cols) local_unnamed_addr #0 {
entry:
  %malloccall = tail call i8* @malloc(i64 16384)
  %img_in_copy = bitcast i8* %malloccall to [16384 x %struct.ap_uint]*
  %malloccall1 = tail call i8* @malloc(i64 16384)
  %img_out_copy = bitcast i8* %malloccall1 to [16384 x %struct.ap_uint]*
  %0 = bitcast %struct.ap_uint* %img_in to [16384 x %struct.ap_uint]*
  %1 = bitcast %struct.ap_uint* %img_out to [16384 x %struct.ap_uint]*
  call fastcc void @copy_in([16384 x %struct.ap_uint]* %0, [16384 x %struct.ap_uint]* %img_in_copy, [16384 x %struct.ap_uint]* %1, [16384 x %struct.ap_uint]* %img_out_copy)
  %2 = getelementptr inbounds [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %img_in_copy, i32 0, i32 0
  %3 = getelementptr inbounds [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %img_out_copy, i32 0, i32 0
  call void @apatb_fast_accel_hw(%struct.ap_uint* %2, i32 %threshold, %struct.ap_uint* %3, i32 %rows, i32 %cols)
  call fastcc void @copy_out([16384 x %struct.ap_uint]* %0, [16384 x %struct.ap_uint]* %img_in_copy, [16384 x %struct.ap_uint]* %1, [16384 x %struct.ap_uint]* %img_out_copy)
  tail call void @free(i8* %malloccall)
  tail call void @free(i8* %malloccall1)
  ret void
}

declare noalias i8* @malloc(i64) local_unnamed_addr

; Function Attrs: noinline
define internal fastcc void @copy_in([16384 x %struct.ap_uint]*, [16384 x %struct.ap_uint]* noalias, [16384 x %struct.ap_uint]*, [16384 x %struct.ap_uint]* noalias) unnamed_addr #1 {
entry:
  call fastcc void @onebyonecpy_hls.p0a16384struct.ap_uint([16384 x %struct.ap_uint]* %1, [16384 x %struct.ap_uint]* %0)
  call fastcc void @onebyonecpy_hls.p0a16384struct.ap_uint([16384 x %struct.ap_uint]* %3, [16384 x %struct.ap_uint]* %2)
  ret void
}

; Function Attrs: noinline
define internal fastcc void @onebyonecpy_hls.p0a16384struct.ap_uint([16384 x %struct.ap_uint]* noalias, [16384 x %struct.ap_uint]* noalias) unnamed_addr #2 {
entry:
  %2 = icmp eq [16384 x %struct.ap_uint]* %0, null
  %3 = icmp eq [16384 x %struct.ap_uint]* %1, null
  %4 = or i1 %2, %3
  br i1 %4, label %ret, label %copy

copy:                                             ; preds = %entry
  br label %for.loop

for.loop:                                         ; preds = %for.loop.head, %copy
  %for.loop.idx7 = phi i64 [ 0, %copy ], [ %for.loop.idx.next, %for.loop.head ]
  %dst.addr = getelementptr [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %0, i64 0, i64 %for.loop.idx7
  %src.addr = getelementptr [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %1, i64 0, i64 %for.loop.idx7
  %5 = getelementptr [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %1, i64 0, i64 %for.loop.idx7, i32 0, i32 0, i32 0
  %6 = call i1 @fpga_fifo_exist_1(i8* %5)
  br i1 %6, label %7, label %8

; <label>:7:                                      ; preds = %for.loop
  call fastcc void @streamcpy_hls.p0struct.ap_uint(%struct.ap_uint* %dst.addr, %struct.ap_uint* %src.addr)
  br label %for.loop.head

; <label>:8:                                      ; preds = %for.loop
  %src.addr.01 = getelementptr [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %1, i64 0, i64 %for.loop.idx7, i32 0
  %dst.addr.02 = getelementptr [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %0, i64 0, i64 %for.loop.idx7, i32 0
  %9 = call i1 @fpga_fifo_exist_1(i8* %5)
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %8
  call fastcc void @streamcpy_hls.p0struct.ap_int_base(%struct.ap_int_base* %dst.addr.02, %struct.ap_int_base* %src.addr.01)
  br label %for.loop.head

; <label>:11:                                     ; preds = %8
  %src.addr.0.03 = getelementptr [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %1, i64 0, i64 %for.loop.idx7, i32 0, i32 0
  %dst.addr.0.04 = getelementptr [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %0, i64 0, i64 %for.loop.idx7, i32 0, i32 0
  %12 = call i1 @fpga_fifo_exist_1(i8* %5)
  br i1 %12, label %13, label %14

; <label>:13:                                     ; preds = %11
  call fastcc void @streamcpy_hls.p0struct.ssdm_int(%struct.ssdm_int* %dst.addr.0.04, %struct.ssdm_int* %src.addr.0.03)
  br label %for.loop.head

; <label>:14:                                     ; preds = %11
  %dst.addr.0.0.06 = getelementptr [16384 x %struct.ap_uint], [16384 x %struct.ap_uint]* %0, i64 0, i64 %for.loop.idx7, i32 0, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst.addr.0.0.06, i8* align 1 %5, i64 1, i1 false)
  br label %for.loop.head

for.loop.head:                                    ; preds = %14, %13, %10, %7
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx7, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, 16384
  br i1 %exitcond, label %for.loop, label %ret

ret:                                              ; preds = %for.loop.head, %entry
  ret void
}

declare i1 @fpga_fifo_exist_1(i8*) local_unnamed_addr

; Function Attrs: argmemonly noinline
define internal fastcc void @streamcpy_hls.p0struct.ap_uint(%struct.ap_uint* noalias nocapture, %struct.ap_uint* noalias nocapture) unnamed_addr #3 {
entry:
  %2 = alloca %struct.ap_uint
  br label %empty

empty:                                            ; preds = %push, %entry
  %3 = bitcast %struct.ap_uint* %1 to i8*
  %4 = call i1 @fpga_fifo_not_empty_1(i8* %3)
  br i1 %4, label %push, label %ret

push:                                             ; preds = %empty
  %5 = bitcast %struct.ap_uint* %2 to i8*
  %6 = bitcast %struct.ap_uint* %1 to i8*
  call void @fpga_fifo_pop_1(i8* %5, i8* %6)
  %7 = load volatile %struct.ap_uint, %struct.ap_uint* %2
  %8 = bitcast %struct.ap_uint* %2 to i8*
  %9 = bitcast %struct.ap_uint* %0 to i8*
  call void @fpga_fifo_push_1(i8* %8, i8* %9)
  br label %empty, !llvm.loop !5

ret:                                              ; preds = %empty
  %10 = getelementptr inbounds %struct.ap_uint, %struct.ap_uint* %1, i32 0, i32 0, i32 0, i32 0
  %11 = getelementptr inbounds %struct.ap_uint, %struct.ap_uint* %0, i32 0, i32 0, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %11, i8* align 1 %10, i64 1, i1 false)
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #4

; Function Attrs: argmemonly noinline
define internal fastcc void @streamcpy_hls.p0struct.ap_int_base(%struct.ap_int_base* noalias nocapture, %struct.ap_int_base* noalias nocapture) unnamed_addr #3 {
entry:
  %2 = alloca %struct.ap_int_base
  br label %empty

empty:                                            ; preds = %push, %entry
  %3 = bitcast %struct.ap_int_base* %1 to i8*
  %4 = call i1 @fpga_fifo_not_empty_1(i8* %3)
  br i1 %4, label %push, label %ret

push:                                             ; preds = %empty
  %5 = bitcast %struct.ap_int_base* %2 to i8*
  %6 = bitcast %struct.ap_int_base* %1 to i8*
  call void @fpga_fifo_pop_1(i8* %5, i8* %6)
  %7 = load volatile %struct.ap_int_base, %struct.ap_int_base* %2
  %8 = bitcast %struct.ap_int_base* %2 to i8*
  %9 = bitcast %struct.ap_int_base* %0 to i8*
  call void @fpga_fifo_push_1(i8* %8, i8* %9)
  br label %empty, !llvm.loop !7

ret:                                              ; preds = %empty
  %10 = getelementptr inbounds %struct.ap_int_base, %struct.ap_int_base* %1, i32 0, i32 0, i32 0
  %11 = getelementptr inbounds %struct.ap_int_base, %struct.ap_int_base* %0, i32 0, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %11, i8* align 1 %10, i64 1, i1 false)
  ret void
}

; Function Attrs: argmemonly noinline
define internal fastcc void @streamcpy_hls.p0struct.ssdm_int(%struct.ssdm_int* noalias nocapture, %struct.ssdm_int* noalias nocapture) unnamed_addr #3 {
entry:
  %2 = alloca %struct.ssdm_int
  br label %empty

empty:                                            ; preds = %push, %entry
  %3 = bitcast %struct.ssdm_int* %1 to i8*
  %4 = call i1 @fpga_fifo_not_empty_1(i8* %3)
  br i1 %4, label %push, label %ret

push:                                             ; preds = %empty
  %5 = bitcast %struct.ssdm_int* %2 to i8*
  %6 = bitcast %struct.ssdm_int* %1 to i8*
  call void @fpga_fifo_pop_1(i8* %5, i8* %6)
  %7 = load volatile %struct.ssdm_int, %struct.ssdm_int* %2
  %8 = bitcast %struct.ssdm_int* %2 to i8*
  %9 = bitcast %struct.ssdm_int* %0 to i8*
  call void @fpga_fifo_push_1(i8* %8, i8* %9)
  br label %empty, !llvm.loop !8

ret:                                              ; preds = %empty
  %10 = getelementptr inbounds %struct.ssdm_int, %struct.ssdm_int* %1, i32 0, i32 0
  %11 = getelementptr inbounds %struct.ssdm_int, %struct.ssdm_int* %0, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %11, i8* align 1 %10, i64 1, i1 false)
  ret void
}

; Function Attrs: noinline
define internal fastcc void @copy_out([16384 x %struct.ap_uint]*, [16384 x %struct.ap_uint]* noalias, [16384 x %struct.ap_uint]*, [16384 x %struct.ap_uint]* noalias) unnamed_addr #5 {
entry:
  call fastcc void @onebyonecpy_hls.p0a16384struct.ap_uint([16384 x %struct.ap_uint]* %0, [16384 x %struct.ap_uint]* %1)
  call fastcc void @onebyonecpy_hls.p0a16384struct.ap_uint([16384 x %struct.ap_uint]* %2, [16384 x %struct.ap_uint]* %3)
  ret void
}

declare void @free(i8*) local_unnamed_addr

declare void @apatb_fast_accel_hw(%struct.ap_uint*, i32, %struct.ap_uint*, i32, i32)

define void @fast_accel_hw_stub_wrapper(%struct.ap_uint*, i32, %struct.ap_uint*, i32, i32) #6 {
entry:
  %5 = bitcast %struct.ap_uint* %0 to [16384 x %struct.ap_uint]*
  %6 = bitcast %struct.ap_uint* %2 to [16384 x %struct.ap_uint]*
  call void @copy_out([16384 x %struct.ap_uint]* null, [16384 x %struct.ap_uint]* %5, [16384 x %struct.ap_uint]* null, [16384 x %struct.ap_uint]* %6)
  %7 = bitcast [16384 x %struct.ap_uint]* %5 to %struct.ap_uint*
  %8 = bitcast [16384 x %struct.ap_uint]* %6 to %struct.ap_uint*
  call void @fast_accel_hw_stub(%struct.ap_uint* %7, i32 %1, %struct.ap_uint* %8, i32 %3, i32 %4)
  call void @copy_in([16384 x %struct.ap_uint]* null, [16384 x %struct.ap_uint]* %5, [16384 x %struct.ap_uint]* null, [16384 x %struct.ap_uint]* %6)
  ret void
}

declare void @fast_accel_hw_stub(%struct.ap_uint*, i32, %struct.ap_uint*, i32, i32)

declare i1 @fpga_fifo_not_empty_1(i8*)

declare void @fpga_fifo_pop_1(i8*, i8*)

declare void @fpga_fifo_push_1(i8*, i8*)

attributes #0 = { noinline "fpga.wrapper.func"="wrapper" }
attributes #1 = { noinline "fpga.wrapper.func"="copyin" }
attributes #2 = { noinline "fpga.wrapper.func"="onebyonecpy_hls" }
attributes #3 = { argmemonly noinline "fpga.wrapper.func"="streamcpy_hls" }
attributes #4 = { argmemonly nounwind }
attributes #5 = { noinline "fpga.wrapper.func"="copyout" }
attributes #6 = { "fpga.wrapper.func"="stub" }

!llvm.dbg.cu = !{}
!llvm.ident = !{!0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0}
!llvm.module.flags = !{!1, !2, !3}
!blackbox_cfg = !{!4}

!0 = !{!"clang version 7.0.0 "}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.rotate.disable"}
!7 = distinct !{!7, !6}
!8 = distinct !{!8, !6}
