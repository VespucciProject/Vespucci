#include "Data/Analysis/cwtdata.h"
CWTData::CWTData(QSharedPointer<VespucciDataset> dataset)
{
    dataset_ = dataset;
    peak_data_.set_size(dataset_->spectra_ptr()->n_cols);
}

///
/// \brief CWTData::Apply
/// \param wavelet
/// \param max_scale
/// \param gap_threshold
/// \param min_ridge_length
/// \param search_width
/// \param noise_threshold
/// \param noise_method
/// \param noise_window_size
/// \param save_coefs
/// \param save_coef_plots
/// \param save_ridge_plots
/// \param save_ridge_plot_data
/// \param save_directory
///
void CWTData::Apply(string wavelet,
                    uword max_scale,
                    uword gap_threshold,
                    uword min_ridge_length,
                    uword search_width,
                    double noise_threshold,
                    string noise_method,
                    uword noise_window_size,
                    bool save_coefs,
                    bool save_coef_plots,
                    bool save_ridge_plots,
                    bool save_ridge_plot_data,
                    bool estimate_width,
                    QString save_directory,
                    QString image_format,
                    QCPColorGradient gradient)
{

    uvec scales(max_scale);
    for (uword i = 0; i < scales.n_rows; ++i){
        scales(i) = i + 1;
    }
    mat *spectra = dataset_->spectra_ptr();
    vec abscissa = dataset_->wavelength();
    mat counts_temp(spectra->n_rows, 2);
    counts_temp.col(0) = abscissa;
    counts_temp.col(1) = zeros(spectra->n_rows);
    mat data;
    int progress_it = 0;
    //iterate over spectra
    int progress_it_max = (3 + estimate_width + save_coefs + save_coef_plots +
            (save_ridge_plot_data || save_ridge_plots)) * spectra->n_cols;
    QProgressDialog progress("Detecting peaks...", "Cancel", 0, progress_it_max, 0);
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle("Detecting Peaks");
    QString current_description;
    cout << "for loop" << endl;
    for (uword i = 0; i < spectra->n_cols; ++i){
        current_description = "Spectrum " + QString::number(i+1) + " of " + QString::number(spectra->n_cols);
        if (progress.wasCanceled()){
            clear();
            return;
        }

        vec current_spectrum = spectra->col(i);

        progress.setLabelText(current_description + ":\nPerforming CWT...");
        mat coefs = Vespucci::Math::Transform::cwt(current_spectrum, wavelet, scales);

        progress.setValue(++progress_it);
        progress.setLabelText(current_description + ":\nIdentifying Ridges...");

        //perform the analysis
        cout << "call findridges" << endl;
        std::vector<Vespucci::Math::CWTRidge> ridges =
                Vespucci::Math::PeakFinding::FindRidges(coefs,
                                                        scales,
                                                        gap_threshold,
                                                        min_ridge_length,
                                                        search_width,
                                                        noise_threshold,
                                                        noise_method,
                                                        noise_window_size);
        progress.setValue(++progress_it);

        //peform width estimation if requested
        vec abscissa = dataset_->wavelength();
        if (estimate_width){
            cout << "estimatewidth" << endl;
            progress.setLabelText(current_description + ":\nEstimating Width...");
            try{
                Vespucci::Math::PeakFinding::EstimateWidth(current_spectrum, abscissa, ridges);
            }
            catch(exception e){
                cerr << "Failed width estimation" << endl;
                cerr << e.what();
                throw e;
            }
            progress.setValue(++progress_it);
        }
        //
        data.set_size(ridges.size(), (estimate_width ? 4 :3));
        progress.setLabelText(current_description + ":\nFormatting results...");
        std::for_each(ridges.begin(),
                      ridges.end(),
                      [&data, estimate_width, abscissa, &counts_temp](Vespucci::Math::CWTRidge &r)
                        {
                            //applying std::floor to functions of return type
                            //uword because int-double conversion has some weird
                            //artifacts.
                            uword center_ind = r.PeakCenter();
                            rowvec row(4);
                            row(0) = abscissa(center_ind);
                            row(1) = r.SNR();
                            row(2) = std::floor(r.length());

                            if (estimate_width)
                                row(3) = r.width();

                            data.insert_rows(data.n_rows, row);
                            counts_temp(center_ind, 1) += 1.0;
                        });
        progress.setValue(++progress_it);

        //save things if requested:

        //save coefficients if requested
        if (save_coefs){
            progress.setLabelText(current_description + ":\nSaving Coefficients...");
            coefs.save(save_directory.toStdString() + "/coefs_" + std::to_string(i) + ".csv", csv_ascii);
            progress.setValue(++progress_it);
        }
        //plot coefficients if requested
        if (save_coef_plots){
            progress.setLabelText(current_description + ":\nPlotting Coefficients...");
            QCPColorMapData *map_data =
                    new QCPColorMapData(coefs.n_cols,
                                        coefs.n_rows,
                                        dataset_->WavelengthRange(),
                                        QCPRange(1, coefs.n_cols));
            for (uword j1 = 0; j1 < coefs.n_cols; ++j1)
                for (uword i1 = 0; i1 < coefs.n_rows; ++i1)
                    map_data->setCell(j1, i1, coefs(i1,j1));
            QCustomPlot *map_plot = new QCustomPlot();
            QCPColorMap *color_map = new QCPColorMap(map_plot->xAxis, map_plot->yAxis);
            map_plot->addPlottable(color_map);
            color_map->setData(map_data);
            map_plot->xAxis->setLabel(dataset_->x_axis_description());
            map_plot->yAxis->setLabel("Scales");
            color_map->setGradient(gradient);
            color_map->rescaleDataRange(true);
            map_plot->rescaleAxes();
            map_plot->replot();

            //save the plot based on file format
            if (image_format == "tif")
                map_plot->saveRastered(save_directory + "/coef_plot_" + QString::number(i) + ".tif", 0, 0, 1.0, "TIFF");
            else if (image_format == "bmp")
                map_plot->saveBmp(save_directory + "/coef_plot_" + QString::number(i) + ".bmp");
            else if (image_format == "png")
                map_plot->savePng(save_directory + "/coef_plot_" + QString::number(i) + ".png");
            else if (image_format == "jpg")
                map_plot->saveJpg(save_directory + "/coef_plot_" + QString::number(i) + ".jpg");
            else
                map_plot->savePdf(save_directory + "/coef_plot_" + QString::number(i) + ".pdf");
            progress.setValue(++progress_it);
        }//end if (plot)

        //save ridge info if requested
        if (save_ridge_plot_data || save_ridge_plots){
            progress.setLabelText(current_description + ":\nPlotting Ridges...");

            QVector<double> ridge_plot_keys;
            QVector<double> ridge_plot_values;
            std::string current_column;
            std::ofstream ridge_plot_file;
            QCustomPlot *ridge_plot = 0;
            QCPGraph *ridge_graph = 0;

            if (save_ridge_plot_data){
                ridge_plot_file.open(save_directory.toStdString() + "/ridges.csv", ios_base::app);
            }
            if (save_ridge_plots){
                ridge_plot = new QCustomPlot(0);
                ridge_plot->xAxis->setRange(dataset_->WavelengthRange());
                ridge_plot->yAxis->setRange(QCPRange(1, max_scale));
            }

            //iterate over ridges
            for (Vespucci::Math::CWTRidge r : ridges){
                //points.col(0) is index, points.col(1) is scale
                umat points = r.points();

                if (save_ridge_plots){
                    ridge_graph = ridge_plot->addGraph();
                    ridge_graph->setScatterStyle(QCPScatterStyle::ssDisc);
                }
                if (save_ridge_plot_data){
                    current_column = "\nspectra_ " + std::to_string(i) +
                            ",ridge_" + std::to_string(r.id());
                }

                for (uword i1 = 0; i1 < points.n_rows; ++i1){

                    if (save_ridge_plot_data)
                        current_column += std::to_string(abscissa(points(i1, 0))) + "," + std::to_string(points(i1, 1)) + "\n";

                    if (save_ridge_plots){
                        ridge_plot_keys.append(abscissa(points(i1, 0)));
                        ridge_plot_values.append(points(i1, 1));
                    }

                } //end for over points

                if (save_ridge_plot_data){
                    ridge_plot_file << current_column;
                }
                if (save_ridge_plots){
                    //each ridge is considered a separate graph so that they may be linked
                    ridge_graph->addData(ridge_plot_keys, ridge_plot_values);
                }
            }//end for over ridges


            if (save_ridge_plots){
                //save the plot based on file format
                if (image_format == "tif")
                    ridge_plot->saveRastered(save_directory + "/ridge_plot_" + QString::number(i) + ".tif", 0, 0, 1.0, "TIFF");
                else if (image_format == "bmp")
                    ridge_plot->saveBmp(save_directory + "/ridge_plot_" + QString::number(i) + ".bmp");
                else if (image_format == "png")
                    ridge_plot->savePng(save_directory + "/ridge_plot_" + QString::number(i) + ".png");
                else if (image_format == "jpg")
                    ridge_plot->saveJpg(save_directory + "/ridge_plot_" + QString::number(i) + ".jpg");
                else
                    ridge_plot->savePdf(save_directory + "/ridge_plot_" + QString::number(i) + ".pdf");
            }

            if (ridge_plot_file.is_open()){
                ridge_plot_file.close();
            }
            progress.setValue(++progress_it);
        } //end if for saving ridge info
    //add the collected information
    peak_data_(i) = data;
    counts_ = counts_temp;

    }//end for iterating over spectra
    cout << "End of Apply()" << endl;
    //format of centers_ is pos, length, snr, width
    return;
}

///
/// \brief CWTData::centers
/// \return A single matrix containing a summary of the information in peak_data_
/// This is intended to be called once and that matrix stored as an AnalysisResult
/// to be viewed in the data viewer.
///
mat CWTData::centers()
{
    mat centers;
    uword cols = 2 * peak_data_(0).n_cols;

    for (uword pd_i = 0; pd_i < peak_data_.n_elem; ++pd_i){
        for (uword i = 0; i < peak_data_(pd_i).n_rows; ++i){
            double current_center = peak_data_(pd_i)(i, 0);
            uvec q;
            if (centers.n_cols)
                q = find(centers.col(0) == current_center);
            else
                q.set_size(0);

            if (!q.n_rows){
                rowvec row(cols);
                row(0) = peak_data_(pd_i)(i, 0);
                row(1) = 1.0;
                for (uword k = 2; k <cols; k+=2){
                    try{
                        row(k) = peak_data_(pd_i)(i, (k/2));
                        row(k+1) = 0;
                    }catch(exception e){
                        cout << "inside for loop, !q.n_rows: " << e.what();
                        throw e;
                    }
                }
                if (!centers.n_rows)
                    centers = row;
                else
                    centers.insert_rows(centers.n_rows, row);
            }//new row
            else{
                //found a degenerate value
                //increment count
                uword ind;
                double old_count;
                try{
                    ind = q(0);
                    old_count = centers(ind, 1);
                    centers(ind, 1) = old_count + 1.0;
                }catch(exception e){
                    cout << "beginning of else: " << e.what();
                    throw e;
                }

                for(uword k = 2; k < cols; k += 2){
                    double old_avg, old_stdev, new_value;

                    old_avg = centers(ind, k);
                    old_stdev = centers(ind, k+1);
                    new_value = peak_data_(pd_i)(i, (k/2));


                        //calculate new stats
                    double new_avg =
                            Vespucci::Math::RecalculateAverage(new_value,
                                                               old_avg,
                                                               old_count);
                    double new_stdev =
                            Vespucci::Math::RecalculateStdDev(new_value,
                                                              old_avg,
                                                              old_stdev,
                                                              old_count);
                        //replace with new values
                    centers(ind, k) = new_avg;
                    centers(ind, k+1) = new_stdev;
                }//iterate over stats, recalculating

            }//else (degenerate row);
        }//for over rows
    }//for over spectra

    centers.save("C:/Users/Dan/Desktop/centers.csv", csv_ascii);
    return centers;
}


///
/// \brief CWTData::clear
/// Delete everything in peak_data_
void CWTData::clear()
{
    peak_data_.clear();
}

///
/// \brief CWTData::counts
/// \return The matrix describing the counts of peaks at each center
///
mat CWTData::counts() const
{
    return counts_;
}

///
/// \brief CWTData::HasPeaks
/// \param range_list A matrix where each row consists of a left and right bound
/// to determine whether or not a peak center was found in that range.
/// \return A matrix with the number of rows equal to the rows of the dataset
/// when this object was created. Each column corresponds to a requested range.
///
mat CWTData::HasPeaks(const mat &range_list)
{
    mat report(peak_data_.n_elem, range_list.n_rows);
    for (uword spec_it = 0; spec_it < peak_data_.n_elem; ++spec_it){
        arma::vec current_centers = peak_data_(spec_it).col(0);
        for (uword range_it = 0; range_it < range_list.n_rows; ++range_it){
            uvec q = find(
                         (current_centers >= range_list(range_it, 0) )
                          && (current_centers <= range_list(range_it, 1)));
            if (q.n_elem)
                report(spec_it, range_it) = 1.0;
            else
                report(spec_it, range_it) = 0;
        }
    }

    return report;
}
