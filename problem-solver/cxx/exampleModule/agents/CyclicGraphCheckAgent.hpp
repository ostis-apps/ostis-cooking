#pragma once

#include <sc-memory/kpm/sc_agent.hpp>

#include "keynodes/keynodes.hpp"
#include "CyclicGraphCheckAgent.generated.hpp"

namespace exampleModule
{

    class CyclicGraphCheckAgent : public ScAgent
    {
        SC_CLASS(Agent, Event(Keynodes::question_check_cyclic_graph, ScEvent::Type::AddOutputEdge));
        SC_GENERATED_BODY();

    private:
        ScAddr prevNode;
        bool exist(ScAddr node, std::vector<ScAddr> nodes);
        std::vector<ScAddr> getAllNodes(ScAddr startNode);
        void DFS(std::vector<ScAddr> container, std::vector<std::vector<ScAddr>>& cycle_list, std::vector<ScAddr> id_list, ScAddr initial_id, ScAddr current_id);
        std::vector<std::vector<ScAddr>> getCycleList(ScAddr startNode);
        void checkCyclicGraph(ScAddr startNode);
    };

} // namespace exampleModule