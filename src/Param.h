#pragma once

double Fitness(const Algorithm::DataSet<short>::Type& p_crInput, const Algorithm::DataSet<short>::Type& p_crOutput);
bool Stop(const int p_ciIteration, const double p_cdFitness, const double p_cdMin, const double p_cdMax);