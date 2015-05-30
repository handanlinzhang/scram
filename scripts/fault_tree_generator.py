#!/usr/bin/env python
"""fault_tree_generator.py

This script generates a fault tree of various complexities. The generated
fault tree can be put into XML file with OpenPSA MEF ready for analysis
or into a shorthand format file. The resulting fault tree is topologically
sorted.

This script helps create complex fault trees in a short time
to test other analysis tools, for example, input dependent performance
analysis.
"""
from __future__ import print_function, division

from collections import deque
from math import log
import random

import argparse as ap

from shorthand_to_xml import toposort_gates


class Node(object):
    """Representation of a base class for a node in a fault tree.

    Attributes:
        name: A specific name that identifies this node.
        parents: A set of parents of this node.
    """
    def __init__(self, name, parent=None):
        self.name = name
        self.parents = set()
        if parent:
            parent.add_child(self)

    def is_shared(self):
        """Indicates if this node appears in several places."""
        return len(self.parents) > 1

    def num_parents(self):
        """Returns the number of unique parents."""
        return len(self.parents)


class Gate(Node):
    """Representation of a gate of a fault tree.

        Names are assigned sequentially starting from G0.
        G0 is assumed to be the root gate.

    Attributes:
        num_gates: Total number of gates created.
        gates: A set of all gates that are created for the fault tree.
        b_children: Children of this gate that are basic events.
        h_children: Children of this gate that are house events.
        g_children: Children of this gate that are gates.
        gate_type: Type of the gate. Chosen randomly.
        k_num: K number for a K/N combination gate. Chosen randomly.
        mark: Marking for various algorithms.
    """
    num_gates = 0  # to keep track of gates and to name them
    gates = []  # container for all created gates

    def __init__(self, parent=None):
        super(Gate, self).__init__("G" + str(Gate.num_gates), parent)
        Gate.num_gates += 1  # post-decrement to account for the root gate
        self.b_children = set()  # children that are basic events
        self.h_children = set()  # children that are house events
        self.g_children = set()  # children that are gates
        self.k_num = None
        self.gate_type = Factors.get_random_type()
        Gate.gates.append(self)  # keep track of all gates
        self.mark = None

    def num_children(self):
        """Returns the number of children."""
        return (len(self.b_children) + len(self.h_children) +
                len(self.g_children))

    def add_child(self, child):
        """Adds child into a collection of gate or primary event children.

        Note that this function also updates the parent set of the child.

        Args:
            child: Gate, HouseEvent, or BasicEvent child.
        """
        child.parents.add(self)
        if type(child) is Gate:
            self.g_children.add(child)
        elif type(child) is BasicEvent:
            self.b_children.add(child)
        elif type(child) is HouseEvent:
            self.h_children.add(child)

    def get_ancestors(self):
        """Collects ancestors from this gate.

        Returns:
            A set of ancestors.
        """
        ancestors = set([self])
        parents = deque(self.parents)
        while parents:
            parent = parents.popleft()
            if parent not in ancestors:
                ancestors.add(parent)
                parents.extend(parent.parents)
        return ancestors


class BasicEvent(Node):
    """Representation of a basic event in a fault tree.

        Names are assigned sequentially starting from E1.

    Attributes:
        num_basic: Total number of basic events created.
        min_prob: Lower bound of the distribution.
        max_prob: Upper bound of the distribution.
        prob: Probability of failure of this basic event. Assigned randomly.
        basic_events: A list of all basic events created for the fault tree.
        non_ccf_events: A list of basic events that not in CCF groups.
    """
    num_basic = 0
    min_prob = 0
    max_prob = 1
    basic_events = []  # container for created basic events
    non_ccf_events = []  # basic events that are not in ccf groups
    def __init__(self, parent=None):
        BasicEvent.num_basic += 1
        super(BasicEvent, self).__init__("E" + str(BasicEvent.num_basic),
                                           parent)
        self.prob = random.uniform(BasicEvent.min_prob, BasicEvent.max_prob)
        BasicEvent.basic_events.append(self)


class HouseEvent(Node):
    """Representation of a house event in a fault tree.

    Names are assigned sequentially starting from H1.

    Attributes:
        num_house: Total number of house events created.
        state: The state of the house event.
        house_events: A list of all house events created for the fault tree.
    """
    num_house = 0
    house_events = []  # container for created house events
    def __init__(self, parent=None):
        HouseEvent.num_house += 1
        super(HouseEvent, self).__init__("H" + str(HouseEvent.num_house),
                                         parent)
        self.state = random.choice(["true", "false"])
        HouseEvent.house_events.append(self)


class CcfGroup(object):
    """Representation of CCF groups in a fault tree.

    Names are assinged sequentially starting from CCF1.

    Attributes:
        num_ccf: The total number of CCF groups.
        ccf_groups: A collection of created CCF groups.
        name: The name of an instance CCF group.
        prob: Probability for a CCF group.
        model: The CCF model chosen for a group.
        members: A collection of members in a CCF group.
    """
    num_ccf = 0
    ccf_groups = []
    def __init__(self):
        CcfGroup.num_ccf += 1
        self.name = "CCF" + str(CcfGroup.num_ccf)
        self.members = []
        self.prob = random.uniform(BasicEvent.min_prob, BasicEvent.max_prob)
        self.model = "MGL"
        CcfGroup.ccf_groups.append(self)

    def get_factors(self):
        """Generates CCF factors.

        Returns:
            A list of CCF factors.
        """
        assert len(self.members) > 1
        levels = random.randint(2, len(self.members))
        factors = [random.uniform(0.1, 1) for _ in range(levels - 1)]
        return factors


class Factors(object):
    """Collection of factors that determine the complexity of the fault tree.

    This collection must be setup and updated before the fault tree generation
    processes.

    Attributes:
        num_basics: The number of basic events.
        common_b: The percentage of common basic events per gate.
        common_g: The percentage of common gates per gate.
        avg_children: The average number of children for gates.
        parents_b: The average number of parents for common basic events.
        parents_g: The average number of parents for common gates.
    """
    # Factors from the arguments
    num_basics = None
    common_b = None
    common_g = None
    avg_children = None
    parents_b = None
    parents_g = None
    __weights_g = None  # should not be set directly

    # Constant configurations
    __gate_types = ["and", "or", "atleast", "not", "xor"]

    # Calculated factors
    __norm_weights = []  # normalized weights
    __cum_dist = []  # CDF from the weights of the gate types
    __max_children = None  # the upper bound for the number of children
    __ratio = None  # basic events to gates ratio per gate
    __percent_basics = None  # percentage of basic events in gate children
    __percent_gates = None  # percentage of gates in gate children

    @staticmethod
    def __calculate_max_children(avg_children, weights):
        """Calculates the maximum number of children for sampling.

        Args:
            avg_children: The average number of children for gates.
            weights: Normalized weights for gate types.

        Returns:
            The upper bound for sampling in symmetric distributions.
        """
        # Min numbers for AND, OR, K/N, NOT, XOR types.
        min_children = [2, 2, 3, 1, 2]
        # Note that max and min numbers are the same for NOT and XOR.
        const_children = min_children[3:]
        const_weights = weights[3:]
        const_contrib = [x * y for x, y in zip(const_children, const_weights)]
        # AND, OR, K/N gate types can have the varying number of children.
        var_children = min_children[:3]
        var_weights = weights[:3]
        var_contrib = [x * y for x, y in zip(var_children, var_weights)]
        # AND, OR, K/N gate types can have the varying number of children.
        # Since the distribution is symmetric, the average is (max + min) / 2.
        return ((2*avg_children - sum(var_contrib) - 2*sum(const_contrib)) /
                sum(var_weights))

    @staticmethod
    def calculate():
        """Calculates any derived factors from the setup.

        This function must be called after all public factors are initialized.
        """
        Factors.__max_children = Factors.__calculate_max_children(
                Factors.avg_children,
                Factors.__norm_weights)
        Factors.__ratio = Factors.avg_children * (1 - Factors.common_g) - 1
        Factors.__percent_basics = Factors.__ratio / (1 + Factors.__ratio)
        Factors.__percent_gates = 1 / (1 + Factors.__ratio)

    @staticmethod
    def get_weights():
        """Provides weights for gate types.

        Returns:
            Expected to return weights from the arguments.
        """
        assert Factors.__weights_g is not None
        return Factors.__weights_g

    @staticmethod
    def set_weights(weights):
        """Updates gate type weights.

        Args:
            weights: Weights of gate types.
        """
        assert len(weights) == len(Factors.__gate_types)
        assert sum(weights) > 0
        Factors.__weights_g = weights[:]
        Factors.__norm_weights = [x / sum(Factors.__weights_g)
                                  for x in Factors.__weights_g]
        Factors.__cum_dist = Factors.__norm_weights[:]
        Factors.__cum_dist.insert(0, 0)
        for i in range(1, len(Factors.__cum_dist)):
            Factors.__cum_dist[i] += Factors.__cum_dist[i - 1]

    @staticmethod
    def get_random_type():
        """Samples the gate type.

        Returns:
            A randomly chosen gate type.
        """
        r_num = random.random()
        bin_num = 1
        while Factors.__cum_dist[bin_num] <= r_num:
            bin_num += 1
        return Factors.__gate_types[bin_num - 1]

    @staticmethod
    def get_num_children(gate):
        """Randomly selects the number of children for the given gate type.

        This function also has a side effect. It sets k_num for the K/N type of
        gates depending on the number of children.

        Args:
            gate: The parent gate for children.

        Returns:
            Random number of children.
        """
        if gate.gate_type == "not":
            return 1
        elif gate.gate_type == "xor":
            return 2

        max_children = int(Factors.__max_children)
        # Dealing with the fractional part.
        if random.random() < (Factors.__max_children - max_children):
            max_children += 1

        if gate.gate_type == "atleast":
            if max_children < 3:
                max_children = 3
            num_children = random.randint(3, max_children)
            gate.k_num = random.randint(2, num_children - 1)

        return random.randint(2, max_children)

    @staticmethod
    def get_percent_gates():
        """Returns the percentage of gates that should be in children."""
        return Factors.__percent_gates

    @staticmethod
    def get_num_gates(num_basics):
        """Approximates the number of gates in the resulting fault tree.

        This is an estimate of the number of gates needed to initialize the
        fault tree with the given number of basic events and the fault tree
        properties.

        Args:
            num_basics: The number of basic event to get initialized.

        Returns:
            The number of gates needed for the given basic events.
        """
        return (num_basics /
                (Factors.__percent_basics * Factors.avg_children *
                    (1 - Factors.common_b)))

    @staticmethod
    def get_num_common_basics(num_gates):
        """Estimates the number of common basic events.

        These common basic events must be chosen from the total number of
        basic events in order to ensure the average number of parents.

        Args:
            num_gates: The total number of gates in the future fault tree

        Returns:
            The estimated number of common basic events.
        """
        return (Factors.common_b * Factors.__percent_basics *
                Factors.avg_children * num_gates / Factors.parents_b)

    @staticmethod
    def get_num_common_gates(num_gates):
        """Estimates the number of common gates.

        These common gates must be chosen from the total number of
        gates in order to ensure the average number of parents.

        Args:
            num_gates: The total number of gates in the future fault tree

        Returns:
            The estimated number of common gates.
        """
        return (Factors.common_g * Factors.__percent_gates *
                Factors.avg_children * num_gates / Factors.parents_g)


def init_gates(args, gates_queue):
    """Initializes gates and other basic events.

    Args:
        args: Configurations for the fault tree.
        gates_queue: A deque of gates to be initialized.
    """
    # Get an intermediate gate to intialize breadth-first
    gate = gates_queue.popleft()

    num_children = Factors.get_num_children(gate)

    ancestors = None  # needed for cycle prevention
    no_common_g = False  # special corner case with no reuse of gates
    max_trials = len(Gate.gates)

    while gate.num_children() < num_children:
        # Case when the number of basic events is already satisfied
        if len(BasicEvent.basic_events) == args.basics:
            # Reuse already initialized basic events
            gate.add_child(random.choice(BasicEvent.basic_events))
            continue

        # Sample gates-to-basic-events ratio
        s_ratio = random.random()
        s_reuse = random.random()  # sample the reuse frequency
        if s_ratio < Factors.get_percent_gates():
            # Create a new gate or reuse an existing one
            if s_reuse < args.common_g and not no_common_g:
                # Lazy evaluation of ancestors
                if not ancestors:
                    ancestors = gate.get_ancestors()

                for _ in range(max_trials):
                    random_gate = random.choice(Gate.gates)
                    if (random_gate in gate.g_children or
                            random_gate is gate):
                        continue
                    if (not random_gate.g_children or
                            random_gate not in ancestors):
                        gate.add_child(random_gate)
                        break
                # no_common_g = True
            else:
                gates_queue.append(Gate(gate))
        else:
            # Create a new basic event or reuse an existing one
            if s_reuse < args.common_b and BasicEvent.basic_events:
                # Reuse an already initialized basic event
                gate.add_child(random.choice(BasicEvent.basic_events))
            else:
                BasicEvent(gate)

    # Corner case when not enough new basic events initialized, but
    # there are no more intermediate gates to use due to a big ratio
    # or just random accident.
    if not gates_queue and len(BasicEvent.basic_events) < args.basics:
        # Initialize more gates by randomly choosing places in the
        # fault tree.
        random_gate = random.choice(Gate.gates)
        while (random_gate.gate_type == "not" or
                random_gate.gate_type == "xor"):
            random_gate = random.choice(Gate.gates)
        gates_queue.append(Gate(random_gate))

def generate_fault_tree(args):
    """Generates a fault tree of specified complexity from command-line
    arguments.

    Args:
        args: Configurations for fault tree construction.

    Returns:
        Top gate of the created fault tree.
    """
    # Start with a top event
    top_event = Gate()
    while top_event.gate_type != "and" and top_event.gate_type != "or":
        top_event.gate_type = Factors.get_random_type()
    top_event.name = args.root

    # Container for not yet initialized gates
    # A deque is used to traverse the tree breadth-first
    gates_queue = deque()
    gates_queue.append(top_event)

    # Procede with children gates
    while gates_queue:
        init_gates(args, gates_queue)

    # Distribute house events
    while len(HouseEvent.house_events) < args.house:
        target_gate = random.choice(Gate.gates)
        if (target_gate is not top_event and
                target_gate.gate_type != "xor" and
                target_gate.gate_type != "not"):
            HouseEvent(target_gate)

    # Create CCF groups from the existing basic events.
    if args.ccf:
        members = BasicEvent.basic_events[:]
        random.shuffle(members)
        first_mem = 0
        last_mem = 0
        while len(CcfGroup.ccf_groups) < args.ccf:
            last_mem = first_mem + random.randint(2, 2 * args.children - 2)
            if last_mem > len(members):
                break
            CcfGroup().members = members[first_mem : last_mem]
            first_mem = last_mem
        BasicEvent.non_ccf_events = members[first_mem : ]

    return top_event


def write_info(args):
    """Writes the information about the setup and generated fault tree.

    This function uses the output destination from the arguments.

    Args:
        args: Command-line configurations.
    """
    t_file = open(args.out, "w")
    t_file.write("<?xml version=\"1.0\"?>\n")
    t_file.write(
            "<!--\nThis is an autogenerated fault tree description\n"
            "with the following parameters:\n\n"
            "The output file name: " + args.out + "\n"
            "The fault tree name: " + args.ft_name + "\n"
            "The root gate name: " + args.root + "\n\n"
            "The seed of the random number generator: " + str(args.seed) + "\n"
            "The number of basic events: " + str(args.basics) + "\n"
            "The number of house events: " + str(args.house) + "\n"
            "The number of CCF groups: " + str(args.ccf) + "\n"
            "The average number of children for gates: " +
            str(args.children) + "\n"
            "The weights of gate types [AND, OR, K/N, NOT, XOR]: " +
            str(Factors.get_weights()) + "\n"
            "Percentage of common basic events per gate: " +
            str(args.common_b) + "\n"
            "Percentage of common gates per gate: " + str(args.common_g) + "\n"
            "The avg. number of parents for common basic events: " +
            str(args.parents_b) + "\n"
            "The avg. number of parents for common gates: " +
            str(args.parents_g) + "\n"
            "Maximum probability for basic events: " + str(args.maxprob) + "\n"
            "Minimum probability for basic events: " + str(args.minprob) + "\n"
            "-->\n"
            )

    shared_p = [x for x in BasicEvent.basic_events if x.is_shared()]
    shared_g = [x for x in Gate.gates if x.is_shared()]
    and_gates = [x for x in Gate.gates if x.gate_type == "and"]
    or_gates = [x for x in Gate.gates if x.gate_type == "or"]
    atleast_gates = [x for x in Gate.gates if x.gate_type == "atleast"]
    not_gates = [x for x in Gate.gates if x.gate_type == "not"]
    xor_gates = [x for x in Gate.gates if x.gate_type == "xor"]

    frac_b = 0  # fraction of basic events in children per gate
    common_b = 0
    common_g = 0
    for gate in Gate.gates:
        frac_b += len(gate.b_children) / \
                (len(gate.g_children) + len(gate.b_children))
        if gate.b_children:
            common_b += len([x for x in gate.b_children if x.is_shared()]) / \
                    len(gate.b_children)
        if gate.g_children:
            common_g += len([x for x in gate.g_children if x.is_shared()]) / \
                    len(gate.g_children)
    num_gates = len(Gate.gates)
    common_b = common_b / num_gates
    common_g = common_g / num_gates
    frac_b = frac_b / num_gates

    t_file.write(
            "<!--\nThe generated fault tree has the following metrics:\n\n"
            "The number of basic events: %d" % BasicEvent.num_basic + "\n"
            "The number of house events: %d" % HouseEvent.num_house + "\n"
            "The number of CCF groups: %d" % CcfGroup.num_ccf + "\n"
            "The number of gates: %d" % Gate.num_gates + "\n"
            "    AND gates: %d" % len(and_gates) + "\n"
            "    OR gates: %d" % len(or_gates) + "\n"
            "    K/N gates: %d" % len(atleast_gates) + "\n"
            "    NOT gates: %d" % len(not_gates) + "\n"
            "    XOR gates: %d" % len(xor_gates) + "\n"
            "Basic events to gates ratio: %f" %
            (BasicEvent.num_basic / Gate.num_gates) + "\n"
            "The average number of children for gates: %f" %
            (sum(x.num_children() for x in Gate.gates) / len(Gate.gates)) + "\n"
            "The number of common basic events: %d" % len(shared_p) + "\n"
            "The number of common gates: %d" % len(shared_g) + "\n"
            "Percentage of common basic events per gate: %f" % common_b + "\n"
            "Percentage of common gates per gate: %f" % common_g + "\n"
            "Percentage of children that are basic events per gate: %f" %
            frac_b + "\n"
            )
    if shared_p:
        t_file.write(
                "The avg. number of parents for common basic events: %f" %
                (sum(x.num_parents() for x in shared_p) / len(shared_p)) + "\n")
    if shared_g:
        t_file.write(
                "The avg. number of parents for common gates: %f" %
                (sum(x.num_parents() for x in shared_g) / len(shared_g)) + "\n")

    t_file.write("-->\n\n")

def write_model_data(t_file, basic_events):
    """Appends model data with primary event descriptions.

    Args:
        t_file: The output stream.
        basic_events: A set of basic events.
    """
    # Print probabilities of basic events
    t_file.write("<model-data>\n")

    for basic in basic_events:
        t_file.write("<define-basic-event name=\"" + basic.name + "\">\n"
                     "<float value=\"" + str(basic.prob) + "\"/>\n"
                     "</define-basic-event>\n")

    for house in HouseEvent.house_events:
        t_file.write("<define-house-event name=\"" + house.name + "\">\n"
                     "<constant value=\"" + house.state + "\"/>\n"
                     "</define-house-event>\n")


    t_file.write("</model-data>\n")

def write_results(args, top_event):
    """Writes results of a generated fault tree into an XML file.

    Writes the information about the fault tree in an XML file.
    The fault tree is printed breadth-first.
    The output XML file is not formatted for human readability.

    Args:
        args: Configurations of this fault tree generation process.
        top_event: Top gate of the generated fault tree.
    """
    # Plane text is used instead of any XML tools for performance reasons.
    write_info(args)
    t_file = open(args.out, "a")

    t_file.write("<opsa-mef>\n")
    t_file.write("<define-fault-tree name=\"%s\">\n" % args.ft_name)

    def write_gate(gate, o_file):
        """Print children for the gate.

        Note that it also updates the queue of gates.

        Args:
            gate: The gate to be printed.
            o_file: The output file stream.
        """

        o_file.write("<define-gate name=\"" + gate.name + "\">\n")
        o_file.write("<" + gate.gate_type)
        if gate.gate_type == "atleast":
            o_file.write(" min=\"" + str(gate.k_num) +"\"")
        o_file.write(">\n")
        # Print children that are gates.
        for g_child in gate.g_children:
            o_file.write("<gate name=\"" + g_child.name + "\"/>\n")

        # Print children that are basic events.
        for b_child in gate.b_children:
            o_file.write("<basic-event name=\"" + b_child.name + "\"/>\n")

        # Print children that are house events.
        for h_child in gate.h_children:
            o_file.write("<house-event name=\"" + h_child.name + "\"/>\n")

        o_file.write("</" + gate.gate_type+ ">\n")
        o_file.write("</define-gate>\n")

    sorted_gates = toposort_gates(top_event, Gate.gates)
    for gate in sorted_gates:
        write_gate(gate, t_file)

    # Proceed with ccf groups
    for ccf_group in CcfGroup.ccf_groups:
        t_file.write("<define-CCF-group name=\"" + ccf_group.name + "\""
                     " model=\"" + ccf_group.model + "\">\n"
                     "<members>\n")
        for member in ccf_group.members:
            t_file.write("<basic-event name=\"" + member.name + "\"/>\n")
        t_file.write("</members>\n<distribution>\n<float value=\"" +
                     str(ccf_group.prob) + "\"/>\n</distribution>\n")
        t_file.write("<factors>\n")
        factors = ccf_group.get_factors()
        level = 2
        for factor in factors:
            t_file.write("<factor level=\"" + str(level) + "\">\n"
                         "<float value=\"" + str(factor) + "\"/>\n</factor>\n")
            level += 1

        t_file.write("</factors>\n")
        t_file.write("</define-CCF-group>\n")

    t_file.write("</define-fault-tree>\n")

    if args.ccf:
        write_model_data(t_file, BasicEvent.non_ccf_events)
    else:
        write_model_data(t_file, BasicEvent.basic_events)

    t_file.write("</opsa-mef>")
    t_file.close()

def write_shorthand(args, top_event):
    """Writes the results into the shorthand format file.

    Note that the shorthand format does not support advanced gates and groups.

    Args:
        args: Configurations for the generation.
        top_event: The top gate of the generated fault tree.
    """
    t_file = open(args.out, "w")
    t_file.write(args.ft_name + "\n\n")

    def write_gate(gate, o_file):
        """Print children for the gate.

        Note that it also updates the queue of gates.

        Args:
            gate: The gate to be printed.
            o_file: The output file stream.
        """
        line = [gate.name]
        line.append(" := ")
        div = ""
        line_end = ""
        if gate.gate_type == "and":
            line.append("(")
            line_end = ")"
            div = " & "
        elif gate.gate_type == "or":
            line.append("(")
            line_end = ")"
            div = " | "
        elif gate.gate_type == "atleast":
            line.append("@(" + str(gate.k_num) + ", [")
            line_end = "])"
            div = ", "
        elif gate.gate_type == "not":
            line.append("~")
            line_end = ""
            div = ""
        elif gate.gate_type == "xor":
            line.append("(")
            line_end = ")"
            div = " ^ "

        first_child = True
        # Print children that are gates.
        for g_child in gate.g_children:
            if first_child:
                line.append(g_child.name)
                first_child = False
            else:
                line.append(div + g_child.name)

        # Print children that are basic events.
        for b_child in gate.b_children:
            if first_child:
                line.append(b_child.name)
                first_child = False
            else:
                line.append(div + b_child.name)

        line.append(line_end)
        o_file.write("".join(line))
        o_file.write("\n")

    sorted_gates = toposort_gates(top_event, Gate.gates)
    for gate in sorted_gates:
        write_gate(gate, t_file)

    # Write basic events
    t_file.write("\n")
    for basic in BasicEvent.basic_events:
        t_file.write("p(" + basic.name + ") = " + str(basic.prob) + "\n")


def check_if_positive(desc, val):
    """Verifies that the value is potive or zero for the supplied argument.

    Args:
        desc: The description of the argument from the command-line.
        val: The value of the argument.

    Raises:
        ArgumentTypeError: The value is negative.
    """
    if val < 0:
        raise ap.ArgumentTypeError(desc + " is negative")

def check_if_less(desc, val, ref):
    """Verifies that the value is less than some reference for
    the supplied argument.

    Args:
        desc: The description of the argument from the command-line.
        val: The value of the argument.
        ref: The reference value.

    Raises:
        ArgumentTypeError: The value is more than the reference.
    """
    if val > ref:
        raise ap.ArgumentTypeError(desc + " is more than " + str(ref))

def check_if_more(desc, val, ref):
    """Verifies that the value is more than some reference for
    the supplied argument.

    Args:
        desc: The description of the argument from the command-line.
        val: The value of the argument.
        ref: The reference value.

    Raises:
        ArgumentTypeError: The value is less than the reference.
    """
    if val < ref:
        raise ap.ArgumentTypeError(desc + " is less than " + str(ref))

def manage_cmd_args():
    """Manages command-line description and arguments.

    Returns:
        Arguments that are collected from the command line.

    Raises:
        ArgumentTypeError: There are problemns with the arguments.
    """
    description = "This script creates a fault tree of an arbitrary size and"\
                  " complexity."

    parser = ap.ArgumentParser(description=description,
                               formatter_class=ap.ArgumentDefaultsHelpFormatter)

    ft_name = "name for the fault tree"
    parser.add_argument("--ft-name", type=str, help=ft_name, metavar="NCNAME",
                        default="Autogenerated")

    root = "name for the root gate"
    parser.add_argument("--root", type=str, help=root, default="root",
                        metavar="NCNAME")

    seed = "seed of the pseudo-random number generator"
    parser.add_argument("--seed", type=int, help=seed, default=123,
                        metavar="int")

    basics = "number of basic events"
    parser.add_argument("-b", "--basics", type=int, help=basics, default=100,
                        metavar="int")

    children = "average number of children gates"
    parser.add_argument("-c", "--children", type=float, help=children,
                        default=3.0, metavar="float")

    type_weights = "weights for samling [AND, OR, K/N, NOT, XOR] gate types"
    parser.add_argument("--weights-g", type=str, nargs="+", help=type_weights,
                        default=[1, 1, 0, 0, 0], metavar="float")

    common_b = "percentage of common basic events per gate"
    parser.add_argument("--common-b", type=float, help=common_b, default=0.1,
                        metavar="float")

    common_g = "percentage of common gates per gate"
    parser.add_argument("--common-g", type=float, help=common_g, default=0.1,
                        metavar="float")

    parents_b = "average number of parents for common basic events"
    parser.add_argument("--parents-b", type=float, help=parents_b, default=2.0,
                        metavar="float")

    parents_g = "average number of parents for common gates"
    parser.add_argument("--parents-g", type=float, help=parents_g, default=2.0,
                        metavar="float")

    maxprob = "maximum probability for basic events"
    parser.add_argument("--maxprob", type=float, help=maxprob, default=0.1,
                        metavar="float")

    minprob = "minimum probability for basic events"
    parser.add_argument("--minprob", type=float, help=minprob, default=0.001,
                        metavar="float")

    house = "number of house events"
    parser.add_argument("--house", type=int, help=house, default=0,
                        metavar="int")

    ccf = "number of ccf groups"
    parser.add_argument("--ccf", type=int, help=ccf, default=0, metavar="int")

    out = "specify the output file to write the generated fault tree"
    parser.add_argument("-o", "--out", help=out, type=str,
                        default="fault_tree.xml", metavar="path")

    shorthand = "apply the shorthand format to the output"
    parser.add_argument("--shorthand", action="store_true", help=shorthand)

    args = parser.parse_args()

    # Check for validity of arguments
    check_if_positive(children, args.children)
    check_if_positive(basics, args.basics)
    check_if_positive(minprob, args.minprob)
    check_if_positive(maxprob, args.maxprob)
    check_if_positive(common_b, args.common_b)
    check_if_positive(common_g, args.common_g)
    check_if_positive(parents_b, args.parents_b)
    check_if_positive(parents_g, args.parents_g)
    check_if_positive(house, args.house)
    check_if_positive(ccf, args.ccf)

    check_if_less(common_b, args.common_b, 0.9)
    check_if_less(common_g, args.common_g, 0.9)
    check_if_less(maxprob, args.maxprob, 1)
    check_if_less(minprob, args.minprob, 1)

    check_if_more(children, args.children, 2)
    check_if_more(parents_b, args.parents_b, 2)
    check_if_more(parents_g, args.parents_g, 2)
    return args

def validate_setup(args):
    """Checks if the relationships between arguments are valid for use.

    These checks are important to ensure that the requested fault tree is
    producible and realistic to achieve in reasonable time.

    Args:
        args: Command-line arguments with values.

    Raises:
        ArgumentTypeError: There are problemns with the arguments.
    """
    if args.maxprob < args.minprob:
        raise ap.ArgumentTypeError("Max probability < Min probability")

    if args.house >= args.basics:
        raise ap.ArgumentTypeError("Too many house events")

    if args.ccf > args.basics / args.children:
        raise ap.ArgumentTypeError("Too many ccf groups")

    if args.weights_g:
        if [i for i in args.weights_g if float(i) < 0]:
            raise ap.ArgumentTypeError("weights cannot be negative")

        if len(args.weights_g) > 5:
            raise ap.ArgumentTypeError("too many weights are provided")

        weights_float = [float(i) for i in args.weights_g]
        if sum(weights_float) == 0:
            raise ap.ArgumentTypeError("atleast one non-zero weight is needed")

    if args.shorthand:
        if args.out == "fault_tree.xml":
            args.out = "fault_tree.txt"
        if args.house:
            raise ap.ArgumentTypeError("No house event representation "
                                       "for the shorthand format")

def setup_factors(args):
    """Configures the fault generation by assigning factors.

    Args:
        args: Command-line arguments with values for factors.

    Raises:
        ArgumentTypeError: There are problemns with the arguments.
    """
    validate_setup(args)
    random.seed(args.seed)
    BasicEvent.min_prob = args.minprob
    BasicEvent.max_prob = args.maxprob
    Factors.num_basics = args.basics
    Factors.common_b = args.common_b
    Factors.common_g = args.common_g
    Factors.parents_b = args.parents_b
    Factors.parents_g = args.parents_g
    Factors.avg_children = args.children

    weights_float = [float(i) for i in args.weights_g]
    for i in range(5 - len(weights_float)):
        weights_float.append(0)
    Factors.set_weights(weights_float)

    Factors.calculate()


def main():
    """The main fuction of the fault tree generator.

    Raises:
        ArgumentTypeError: There are problemns with the arguments.
    """
    args = manage_cmd_args()
    setup_factors(args)

    top_event = generate_fault_tree(args)

    # Write output files
    if args.shorthand:
        write_shorthand(args, top_event)
    else:
        write_results(args, top_event)

if __name__ == "__main__":
    try:
        main()
    except ap.ArgumentTypeError as error:
        print("Argument Error:")
        print(error)
