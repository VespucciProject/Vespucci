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
    uvec scales = conv_to<uvec>::from(linspace(1, max_scale, max_scale));

    mat *spectra = dataset_->spectra_ptr();
    vec abscissa = dataset_->wavelength();
    mat data;

    //iterate over spectra
    for (uword i = 0; i < spectra->n_cols; ++i){
        vec current_spectrum = spectra->col(i);
        mat coefs = Vespucci::Math::Transform::cwt(current_spectrum, wavelet, scales);


        //perform the analysis
        std::vector<Vespucci::Math::CWTRidge> ridges =
                Vespucci::Math::PeakFinding::FindRidges(coefs,
                                                        scales,
                                                        gap_threshold,
                                                        min_ridge_length,
                                                        search_width,
                                                        noise_threshold,
                                                        noise_method,
                                                        noise_window_size);
        //peform width estimation if requested
        if (estimate_width){
            Vespucci::Math::PeakFinding::EstimateWidth(current_spectrum, dataset_->wavelength(), ridges);

        }
        //
        data.set_size(ridges.size(), (estimate_width ? 4 : 3));
        std::for_each(ridges.begin(),
                      ridges.end(),
                      [&data, estimate_width](Vespucci::Math::CWTRidge &r)
                        {
                            rowvec row(4);
                            row(0) = r.PeakCenter();
                            row(1) = r.SNR();
                            row(2) = r.length();

                            if (estimate_width)
                                row(3) = r.width();

                            data.insert_rows(data.n_rows, row);
                        });


        //save things if requested:

        //save coefficients if requested
        if (save_coefs){
            coefs.save(save_directory.toStdString() + "/coefs_" + std::to_string(i) + ".csv", csv_ascii);
        }
        //plot coefficients if requested
        if (save_coef_plots){
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
        }//end if (plot)

        //save ridge info if requested
        if (save_ridge_plot_data || save_ridge_plots){
            QVector<double> ridge_plot_keys;
            QVector<double> ridge_plot_values;
            std::string current_column;
            std::ofstream ridge_plot_file;
            QCustomPlot *ridge_plot = 0;
            QCPGraph *ridge_graph = 0;

            if (save_ridge_plot_data){
                ridge_plot_file.open(save_directory.toStdString() + "/ridges.csv");
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

        } //end if for saving ridge info
    //add the collected information
    peak_data_(i) = data;

    }//end for iterating over spectra

    //format of centers_ is pos, length, snr, width
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
    //set centers (default interface of this data through AnalysisResults)
    for (uword i = 0; i < peak_data_.n_elem; ++i){
        for (uword j = 0; j < peak_data_(i).n_rows; ++j){
            //search peak center column for value
            double current = peak_data_(i)(j,0);
            uvec q;
            if(centers.n_rows){q = find(centers.col(0) == current);}
            //q will have size zero if centers is empty

            //add a new row for each found value
            if (q.n_elem == 0){
                rowvec row(cols);
                row(0) = peak_data_(i)(j,0);
                row(1) = 1.0;
                for (uword k = 2; k < cols; k+=2){
                    row(k) = peak_data_(i)(j, (k/2)); //mean of value is value
                    row(k+1) = 0; //stddev of value is 0 (n==1)
                }

                //add data for new row
                if (!centers.n_rows) //evaulates false if no rows (for noncoders...)
                    centers = row; //start matrix with this row
                else
                    centers.insert_rows(centers.n_rows, row); //insert new row
            }//if

            else{ //row already found so increment its count and recalc stats
                double old_count = centers(q(0), 1);
                centers(q(0),1) += 1.0; //increment count

                //recalculate averages and stddevs
                //k=2 ridge length
                //k=4 snr
                //k=6 width (if requested)
                for (uword k = 2; k < centers.n_cols; k+=2){
                    double old_avg = centers(q(0), k);
                    double old_stddev = centers(q(0), k+1);
                    double new_value = peak_data_(i)(j,(k/2));

                    centers(q(0), k) =
                            Vespucci::Math::RecalculateAverage(new_value,
                                                               old_avg,
                                                               old_count);
                    centers(q(0), k+1) =
                            Vespucci::Math::RecalculateStdDev(new_value,
                                                              old_avg,
                                                              old_stddev,
                                                              old_count);
                }//recalculation
            }//else
        }//iteration on this spectrum
    }//iteration through each spectrum to set centers

    return centers;
}//end function
