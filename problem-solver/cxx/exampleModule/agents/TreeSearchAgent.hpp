#pragma once
#include <sc-memory/kpm/sc_agent.hpp>
#include "keynodes/keynodes.hpp"
#include "TreeSearchAgent.generated.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

namespace exampleModule{
    class TreeSearchAgent : public ScAgent
            {

        SC_CLASS(Agent, Event(Keynodes::question_find_tree_search, ScEvent::Type::AddOutputEdge))
        SC_GENERATED_BODY()

        std::pair<bool, bool> checked_for_tree(ScMemoryContext *context, ScAddr Del, std::vector<ScAddr> vertexes, std::vector<ScAddr> edges)
        {
            SC_LOG_DEBUG("checked_for_tree");
            std::pair<bool, bool> REZULT;
            bool A;//if q=p-1   true = 1
            bool B;//connected graph = 1

            //number of vertexes
            int v = 0;
            for(int i = 0; i < vertexes.size(); i++)
                if (!context->HelperCheckEdge(Del, vertexes[i], ScType::EdgeAccess))
                    v++;
            std::cout<<"Number of vertexes is "<<v<<std::endl;
            //number of edges
            int e = 0;
            for(int i = 0; i < edges.size(); i++)
                if (!context->HelperCheckEdge(Del, edges[i], ScType::EdgeAccess))
                    e++;
            std::cout<<"Number of edges is "<<e<<std::endl;

            if (e/2 == (v - 1))
                A = true;
            else
                A = false;

            //check for vertexes with zero degree
            for (int i = 0; i < vertexes.size(); i++)
            {
                int degree = 0;
                if (!context->HelperCheckEdge(Del,vertexes[i],ScType::EdgeAccess))
                {
                    ScIterator3Ptr it = context->Iterator3(vertexes[i], ScType::EdgeDCommonConst, ScType::NodeConst);
                    while (it->Next())
                        if (!context->HelperCheckEdge(Del, it->Get(1), ScType::EdgeAccess))
                            degree++;
                    if (degree == 0)
                    {
                        std::cout<<"Vertex with zero degree is "<<context->HelperGetSystemIdtf(vertexes[i])<<std::endl;
                        B = 0;
                        REZULT.first = A;
                        REZULT.second = B;
                        return REZULT; //return if graph is unconnected
                    }
                    else
                        B = 1;
                }
            }

            REZULT.first = A;
            REZULT.second = B;
            return REZULT;//return if graph connected
        };

        ScAddr find_max_degree(ScMemoryContext *context, std::vector<ScAddr> &checked, std::vector<ScAddr> vertexes, ScAddr Del)
        {
            int max_degree = 0;
            ScAddr max_node;

            if (checked.size() == 0)
            {
                for (int i = 0; i < vertexes.size(); i++)
                {
                    if (!context->HelperCheckEdge(Del, vertexes[i], ScType::EdgeAccess))
                    {
                        int degree = 0;
                        ScIterator3Ptr it = context->Iterator3(vertexes[i], ScType::EdgeDCommonConst,ScType::NodeConst);
                        while (it->Next())
                            if (!context->HelperCheckEdge(Del, it->Get(1), ScType::EdgeAccess))
                                degree++;
                        if (degree > max_degree)
                        {
                            max_degree = degree;
                            max_node = vertexes[i];
                        }
                    }
                }
                std::cout<<"max_node is "<<context->HelperGetSystemIdtf(max_node)<<std::endl;
                return max_node;
            }
            else
            {
                for (int i = 0; i < vertexes.size(); i++)
                {
                    if (!context->HelperCheckEdge(Del, vertexes[i], ScType::EdgeAccess))
                    {
                        int not_repeat = 0;
                        for (int j = 0; j < checked.size(); j++)
                            if (vertexes[i] != checked[j])
                                not_repeat++;
                        if (not_repeat == checked.size())
                        {
                            int degree = 0;
                            ScIterator3Ptr it = context->Iterator3(vertexes[i], ScType::EdgeDCommonConst,ScType::NodeConst);
                            while (it->Next())
                                if (!context->HelperCheckEdge(Del, it->Get(1), ScType::EdgeAccess))
                                    degree++;
                            if (degree > max_degree)
                            {
                                max_degree = degree;
                                max_node = vertexes[i];
                            }
                        }
                    }
                }
                std::cout<<"max_node is "<<context->HelperGetSystemIdtf(max_node)<<std::endl;
                return max_node;
            }
        };

        void try_to_del(ScMemoryContext *context, ScAddr Del, ScAddr ToDel)
        {
            SC_LOG_DEBUG("try_to_del");
            //mark edges and nodes
            context->CreateEdge(ScType::EdgeAccess, Del, ToDel);
            //
            ScIterator3Ptr it1 = context->Iterator3(ToDel, ScType::EdgeDCommon, ScType::NodeConst);
            while (it1->Next())
                context->CreateEdge(ScType::EdgeAccess, Del, it1->Get(1));
            //
            ScIterator3Ptr it2 = context->Iterator3(ScType::NodeConst, ScType::EdgeDCommon, ToDel);
            while (it2->Next())
                context->CreateEdge(ScType::EdgeAccess, Del, it2->Get(1));

        };

        void delete_connection(ScMemoryContext *context, ScAddr Del, ScAddr NotToDel)
        {
            SC_LOG_DEBUG("delete_connection");
            //del ->node
            ScIterator3Ptr it1 = context->Iterator3(Del, ScType::EdgeAccess, NotToDel);
            it1->Next();
            context->EraseElement(it1->Get(1));
            //del edges
            //2-3
            ScIterator3Ptr it2 = context->Iterator3(NotToDel, ScType::EdgeDCommon, ScType::NodeConst);
            while (it2->Next())
            {
                ScIterator3Ptr it3 = context->Iterator3(Del, ScType::EdgeAccess, it2->Get(1));
                it3->Next();
                context->EraseElement(it3->Get(1));
            }
            //4-5
            ScIterator3Ptr it4 = context->Iterator3(ScType::NodeConst, ScType::EdgeDCommon, NotToDel);
            while (it4->Next())
            {
                ScIterator3Ptr it5 = context->Iterator3(Del, ScType::EdgeAccess, it4->Get(1));
                it5->Next();
                context->EraseElement(it5->Get(1));
            }
        };
    };
}// namespace exampleModule