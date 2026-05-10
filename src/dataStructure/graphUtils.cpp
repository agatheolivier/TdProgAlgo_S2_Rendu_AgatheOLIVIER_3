#include "graphUtils.hpp"
#include "weightedGraph.hpp"

#include <vector>
#include <unordered_map>
#include <functional>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>

using DataStructure::WeightedGraph;
using DataStructure::IDType;

void DFS(WeightedGraph const& graph, IDType const start, std::function<void(IDType const)> const& callback, std::function<bool(IDType const)> const& should_explore_neighbors) {
    std::set<IDType> visited {};
    std::stack<IDType> to_visit {};
    to_visit.push(start);
    while (!to_visit.empty()) {
        auto const current { to_visit.top() };
        to_visit.pop();

        if (visited.contains(current)) {
            continue;
        }

        visited.insert(current);
        callback(current);

        if (should_explore_neighbors(current)) {
            for (auto const& edge : graph.get_neighbors(current)) {
                to_visit.push(edge.to);
            }
        }
    }
}

void print_DFS(WeightedGraph const& graph, IDType const start) {
    /* TODO */
}


template<class T>
using min_priority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

// map of node id to pair of cost and parent node id (for path reconstruction)
std::unordered_map<IDType, std::pair<float, IDType>> Dijkstra(WeightedGraph const& graph, IDType const start, IDType const end) {
    //Création du tableau associatif
    std::unordered_map<IDType, std::pair<float, IDType>> cheminFinal {}; //Ne peut pas contenir deux fois un même point comme c'est un order map
    // priority queue of pair of current cost from starting node and current node id
    // this will sort node base on the cost (as pair are ordered with lexicographical order)
    min_priority_queue<std::pair<float, IDType>> to_visit {}; //Trie les éléments pour que le plus intéressant à visiter soit au dessus
    //Exemple si on a B(1), C(2), D(6) comme dans le cours, top retournera B

    to_visit.push({0.0f, start}); //On ajoute le noeud de départ de coût 0, l'équivalent de A(0)

    //Même début que DFS

    while (!to_visit.empty()) { //Tant qu'il reste des noeuds dans la liste
        std::pair<float, IDType> pointAVisiter = to_visit.top(); //Récupère le point le plus proche, comme B(1)
        to_visit.pop(); //L'enlève de la liste des points à visiter

        float cout = pointAVisiter.first; //Coût, 1
        IDType nom = pointAVisiter.second; //point, B

        if (nom == end){
            break; //On s'arrête quand on a trouvé le chemin le plus court vers le point qui nous intéresse
        }

        for (auto const& voisin : graph.get_neighbors(nom)) { //Parcours les voisins du points, C et D

            float nouveauCout = cout + voisin.weight; //Calcul le nouveau cout

            if (!cheminFinal.contains(voisin.to) //Si le point n'a jamais été visité
                || nouveauCout < cheminFinal[voisin.to].first) //Si on trouve un chemin plus court => D(5) au lieu de D(6)
            {
                cheminFinal[voisin.to] = {nouveauCout, nom}; //On l'ajoute au chemin final
                //C'est un std::unordered_map, donc ça écrase la valeur précédente => on garde la valeur plus petite pour la transmettre à la prochaine fonction
                //Pour atteindre D en faisant 5 de distance, il faut passer par C

                to_visit.push({nouveauCout, voisin.to}); //On ajoute l'info dans le tableau
                //Pas grave qu'il y soit deux fois => avec la priorité on va de toute façon choisir le plus petit
            }
        }
    }
    return cheminFinal; //Renvoie le chemin final qui contient uniquement les plus petits chemin pour aller à un point
}

std::vector<IDType> reconstruct_path(std::unordered_map<IDType, std::pair<float, IDType>> const& distances, IDType const start, IDType const end) {
    std::vector<IDType> path { };
    IDType current_node { end };
    while (current_node != start) {
        path.push_back(current_node);

        auto const find_current_node { distances.find(current_node) };
        if(find_current_node == distances.end()) {
            // if we reach a node that is not in the distances map, it means that there is no path from start to end
            return {};
        }
        else {
            // we update the current node to its parent node
            current_node = find_current_node->second.second;
        }
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<IDType> dijkstra_path(WeightedGraph const& graph, IDType const start, IDType const end) {
    return reconstruct_path(Dijkstra(graph, start, end), start, end);
}