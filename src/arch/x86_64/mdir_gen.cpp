#include "arch/x86_64/mdir_gen.h"

using namespace jab;
using namespace x86_64;
using Kind = IRValueKind;
using enum Register;
using enum Opcode;

MDIRGen::MDIRGen(CompileOptions options, Module* module):
	options{options},
	module{module},
	machine_module{nullptr}
{

}

void MDIRGen::compile() {
	gen_module();
}

std::vector<std::byte> MDIRGen::emit_raw_bin() {
	Encoder encode(module);
	return encode.raw_bin();
}


MCModule* MDIRGen::gen_module() {
	auto* mm = new MCModule(module->name);
	for(auto* fn: module->functions) {
		mm->functions.push_back(gen_function(fn));
	}
	return mm;
}

MCFunction* MDIRGen::gen_function(Function* fn) {
	auto* mc_fn = new MCFunction(fn);

	for(auto* block: fn->blocks)
		for(auto ir_inst: block->insts)
			gen_inst(mc_fn, ir_inst);

	return mc_fn;
}

void MDIRGen::gen_inst(MCFunction* mc_fn, IRInst ir_inst) {
	if(is_imm(ir_inst.op))
		gen_imm(mc_fn, ir_inst);
	else if(is_mov(ir_inst.op))
		gen_mov(mc_fn, ir_inst);
	else if(is_bin(ir_inst.op))
		gen_bin(mc_fn, ir_inst);
	else if(is_branch(ir_inst.op))
		gen_branch(mc_fn, ir_inst);
	else if(is_ret(ir_inst.op))
		gen_ret(mc_fn, ir_inst);
	else
		assert(false);
}

void MDIRGen::gen_imm(MCFunction* mc_fn, IRInst ir_inst) {
	auto op = ir_inst.op;
	auto dest = ir_inst.dest;
	auto src1 = ir_inst.src1;
	auto src2 = ir_inst.src2;

	switch(ir_inst.op) {
		case IROp::iconst8:
			append_inst(mc_fn, {
				.op = mov_reg_imm,
				.reg1 = rax,
				.reg2 = none,

				.extra = {
					.imm = src1.imm
				}
			});
		    return;
		case IROp::iconst16:
			append_inst(mc_fn, {
				.op = mov_reg_imm,
				.reg1 = rax,
				.reg2 = none,

				.extra = {
					.imm = src1.imm
				}
			});
		    return;
		case IROp::iconst32:
			append_inst(mc_fn, {
				.op = mov_reg_imm,
				.reg1 = rax,
				.reg2 = none,

				.extra = {
					.imm = src1.imm
				}
			});
			return;
		case IROp::iconst64:
			append_inst(mc_fn, {
				.op = mov_reg_imm,
				.reg1 = rax,
				.reg2 = none,

				.extra = {
					.imm = src1.imm
				}
			});
			return;
		case IROp::fconst32:
			// TODO not implemented yet
			assert(false);
			append_inst(mc_fn, {
				.op = mov_reg_imm,
				.reg1 = rax,
				.reg2 = none,

				.extra = {
					.imm = src1.imm
				}
			});
			return;
		case IROp::fconst64:
			// TODO not implemented yet
			assert(false);
			append_inst(mc_fn, {
				.op = mov_reg_imm,
				.reg1 = rax,
				.reg2 = none,

				.extra = {
					.imm = src1.imm
				}
			});
			return;
		default:
			assert(false);
	}
}

void MDIRGen::gen_mov(MCFunction* mc_fn, IRInst ir_inst) {
		assert(false);
}

void MDIRGen::gen_bin(MCFunction* mc_fn, IRInst ir_inst) {
	auto op = ir_inst.op;
	auto dest = ir_inst.dest;
	auto src1 = ir_inst.src1;
	auto src2 = ir_inst.src2;

	assert(dest.kind != Kind::imm);

	switch(ir_inst.op) {
		case IROp::addi:
			if(src1.kind == Kind::vreg && src2.kind == Kind::vreg)
				append_inst(mc_fn, {
					.op = add,
					.reg1 = rax,
					.reg2 = rcx,
				});
			else if(src1.kind == Kind::vreg && src2.kind == Kind::imm)
				append_inst(mc_fn, {
					.op = add,
					.reg1 = rax,
					.reg2 = none,

					.extra = {
						.imm = src2.imm
					}
				});
			else	// src1.kind == imm
				append_inst(mc_fn, {
					.op = add,
					.reg1 = rax,
					.reg2 = none,

					.extra = {
						.imm = src1.imm
					}
				});
			return;
		case IROp::subi:
		case IROp::muli:
		case IROp::divi:
		case IROp::modi:
	
		case IROp::addf:
		case IROp::subf:
		case IROp::mulf:
		case IROp::divf:
		case IROp::modf:

		case IROp::lt:
		case IROp::lte:
		case IROp::gt:
		case IROp::gte:
		case IROp::eq:
		default:
			assert(false);
	}
	
}

void MDIRGen::gen_branch(MCFunction* mc_fn, IRInst ir_inst) {
	auto op = ir_inst.op;
	auto dest = ir_inst.dest;
	auto src1 = ir_inst.src1;
	auto src2 = ir_inst.src2;

	assert(false);
}

void MDIRGen::gen_call(MCFunction* mc_fn, IRInst ir_inst) {
	auto op = ir_inst.op;
	auto dest = ir_inst.dest;
	auto src1 = ir_inst.src1;
	auto src2 = ir_inst.src2;

	assert(false);
}

void MDIRGen::gen_ret(MCFunction* mc_fn, IRInst ir_inst) {
	auto op = ir_inst.op;
	auto dest = ir_inst.dest;
	auto src1 = ir_inst.src1;
	auto src2 = ir_inst.src2;

	if(ir_inst.src1.kind == Kind::vreg)
		append_inst(mc_fn, {
			.op = mov,
			.reg1 = rax,
			.reg2 = rbx // TODO
		});
	else if(ir_inst.src1.kind == Kind::imm)
		append_inst(mc_fn, {
			.op = mov,
			.reg1 = rax,
			.reg2 = none,

			.extra = {
				.imm = src1.imm
			}
		});
	else
		assert(false);
    
	append_inst(mc_fn, {
		.op = ret
	});
}

void MDIRGen::append_inst(MCFunction* mc_fn, MCInst mc_inst) {
	mc_fn->insts.push_back(mc_inst);
}